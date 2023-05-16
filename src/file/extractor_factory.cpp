#include "file/extractor_factory.hpp"

std::unique_ptr<vocabzer::TextExtractor> vocabzer::ExtractorFactory::operator()(const std::filesystem::path& file_path) {
	return create_extractor(file_path);
}

std::unique_ptr<vocabzer::TextExtractor> vocabzer::ExtractorFactory::create_extractor(const std::filesystem::path& file_path)
{
	auto extension = get_file_extension(file_path);
	auto type = types.find(extension);

	std::unique_ptr<TextExtractor> extractor;

	if (type != types.end()) {
		extractor.reset(type->second(file_path));
	}
	else {
		extractor.reset(create<TextExtractor>(file_path));
	}

	return extractor;
}

std::string vocabzer::ExtractorFactory::get_file_extension(const std::filesystem::path& file_path) const noexcept {
	auto file_name = file_path.string();
	auto extension_div_pose = file_name.rfind('.');

	std::string extension{ "" };

	if (extension_div_pose != std::string::npos) {
		auto last_pose = file_name.size() - 1;
		if (extension_div_pose != last_pose) {
			extension = file_name.substr(extension_div_pose + 1, last_pose - extension_div_pose);
		}
	}

	return extension;
}