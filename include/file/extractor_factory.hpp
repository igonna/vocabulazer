#pragma once

#include <map>
#include <memory>
#include "file/text_extractor.hpp"
#include "file/pdf_extractor.hpp"

namespace vocabzer {

class ExtractorFactory {
	typedef TextExtractor* creation_func(const std::filesystem::path& file_path);

	template<typename T>
	static TextExtractor* create(const std::filesystem::path& file_path) { return new T(file_path); }

public:
	ExtractorFactory() {
		types.insert({ "txt", create<TextExtractor> });
		types.insert({ "pdf", create<PdfExtractor> });
	}
	std::unique_ptr<TextExtractor> operator()(const std::filesystem::path& file_path);
	std::unique_ptr<TextExtractor> create_extractor(const std::filesystem::path& file_path);

private:
	std::string get_file_extension(const std::filesystem::path& file_path) const noexcept;

private:
	std::map<std::string, creation_func*> types;
};

};