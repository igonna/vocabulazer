#include "file/pdf_extractor.hpp"

#include <string>
#include <memory>
#include <map>
#include <thread>
#include <future>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>

vocabzer::PdfExtractor::PdfExtractor(const std::filesystem::path& file_path)
{
    check_file(file_path);
    process_file(file_path);
}

vocabzer::PdfExtractor::PdfExtractor(PdfExtractor&& other) noexcept
{
    bytes = std::move(other.bytes);
}

vocabzer::PdfExtractor& vocabzer::PdfExtractor::operator=(PdfExtractor&& other) noexcept
{
    bytes = std::move(other.bytes);
    return *this;
}

void vocabzer::PdfExtractor::process_file(const std::filesystem::path& file_path)
{
    std::unique_ptr<poppler::document> document{ poppler::document::load_from_file(file_path) };

    if (!document || document->is_locked()) {
        throw std::runtime_error{ "Failed to open file as pdf: " + file_path.string() };
    }

    // calculate chunks sizes
    const auto pages_count = document->pages();
    if (!pages_count) return;

    const uint32_t chunks_count = std::thread::hardware_concurrency();
    if (!chunks_count) {
        uint32_t& t = const_cast<uint32_t&>(chunks_count);
        t = default_threads_count;
    }
    const auto chunk_size = pages_count / chunks_count;
    const auto last_chunk_size = chunk_size + (pages_count % chunk_size);


    std::map<uint32_t, std::promise<std::vector<char>>> chunks;
    for (uint32_t i = 0; i < chunks_count; ++i) {
        chunks[i] = std::promise<std::vector<char>>();
    }

    // returns bytes array of pages from begin-end (chunk) range
    auto create_chunk = [&document](uint32_t begin, uint32_t end) -> std::vector<char> {
        std::vector<char> chunk_bytes;
        for (auto i = begin; i < end; ++i) {
            std::unique_ptr<poppler::page> page{ document->create_page(i) };
            if (page) {
                auto page_bytes = page->text().to_utf8();
                chunk_bytes.insert(chunk_bytes.end(), page_bytes.begin(), page_bytes.end());
            }
        }
        return chunk_bytes;
    };

    std::vector<std::jthread> threads(chunks_count);
    
    // convert chunks to text with threads
    uint32_t chunk_end = 0;
    uint32_t chunk_start = 0;
    for (uint32_t i = 0; i < chunks_count; ++i) {
        if (i + 1 != chunks_count) {
            chunk_end = (i + 1) * chunk_size;
        } else {
            chunk_end = i * chunk_size + last_chunk_size;
        }
        threads[i] = std::jthread([&chunks, &create_chunk](uint32_t chunk_id, uint32_t chunk_start, uint32_t chunk_end) {
            chunks[chunk_id].set_value(create_chunk(chunk_start, chunk_end));
        }, i, chunk_start, chunk_end);
        chunk_start = chunk_end;
    }

    for (auto& [id, promise] : chunks) {
        auto chunk = promise.get_future().get();
        bytes.insert(bytes.end(), chunk.begin(), chunk.end());
    }
}