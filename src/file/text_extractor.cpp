#include "file/text_extractor.hpp"
#include <fstream>

vocabzer::TextExtractor::TextExtractor(const std::filesystem::path& file_path)
{
	std::wifstream file(file_path, std::ios_base::binary);
	
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + file_path.string());
	}

	const auto file_size = std::filesystem::file_size(file_path);

	file_data.resize(file_size);
	file.read(reinterpret_cast<wchar_t*>(file_data.data()), file_size);
}

const std::vector<std::byte>& vocabzer::TextExtractor::get_text() const noexcept
{
	return file_data;
}
