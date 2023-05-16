#pragma once

#include <filesystem>
#include <vector>

namespace vocabzer {

class TextExtractor {
public:
	virtual ~TextExtractor() = default;
	TextExtractor() = default;
	TextExtractor(const std::filesystem::path& file_path);
	explicit TextExtractor(TextExtractor&& other) noexcept;
	TextExtractor& operator=(TextExtractor&& other) noexcept;

	virtual const std::vector<char>& get_text() const noexcept;

protected:
	void check_file(const std::filesystem::path& file_path) const;

private:
	void process_file(const std::filesystem::path& file_path) noexcept;

protected:
	std::vector<char> bytes;
};

}