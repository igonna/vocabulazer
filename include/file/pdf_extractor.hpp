#pragma once

#include "file/text_extractor.hpp"

namespace vocabzer {

class PdfExtractor : public TextExtractor {
public:
    virtual ~PdfExtractor() = default;
    PdfExtractor() = default;
    explicit PdfExtractor(const std::filesystem::path& file_path);
    explicit PdfExtractor(PdfExtractor&& other) noexcept;
    PdfExtractor& operator=(PdfExtractor&& other) noexcept;

private:
    void process_file(const std::filesystem::path& file_path);
    uint16_t default_threads_count = 2;
};
}