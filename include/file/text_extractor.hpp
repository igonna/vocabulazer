#pragma once

#include <filesystem>

namespace vocabzer {

class TextExtractor {
public:
	explicit TextExtractor(const std::filesystem::path& file_path);
	
	const std::vector<std::byte>& get_text() const noexcept;

private:
	std::vector<std::byte> file_data;
};

}