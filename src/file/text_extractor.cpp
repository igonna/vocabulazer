#include "file/text_extractor.hpp"

#include <fstream>
#include <vector>

vocabzer::TextExtractor::TextExtractor(const std::filesystem::path& file_path)
{
	check_file(file_path);
	process_file(file_path);
}

vocabzer::TextExtractor::TextExtractor(TextExtractor&& other) noexcept
{
	bytes = std::move(other.bytes);
}

vocabzer::TextExtractor& vocabzer::TextExtractor::operator=(TextExtractor&& other) noexcept
{
	bytes = std::move(other.bytes);
	return *this;
}

std::vector<char>& vocabzer::TextExtractor::get_text() noexcept
{
	return bytes;
}

void vocabzer::TextExtractor::check_file(const std::filesystem::path& file_path) const
{
	std::basic_ifstream<char> file{ file_path, std::ios::binary };
	if (!file.is_open()) {
		throw std::runtime_error{ "Failed to open file: " + file_path.string() };
	}
}

void vocabzer::TextExtractor::process_file(const std::filesystem::path& file_path) noexcept
{
	std::basic_ifstream<char> file{ file_path, std::ios::binary };
	bytes.assign(std::istreambuf_iterator<char>(file), {});
}