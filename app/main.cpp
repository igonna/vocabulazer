#include <iostream>
#include <fstream>
#include <filesystem>
#include "file/extractor_factory.hpp"
#include "text/text_converter.hpp"

int main(int argc, char* argv[])
{
	if (argc != 3) return 1;

	std::filesystem::path file_path{ argv[1] };
	std::filesystem::path file_path_out{ argv[2] };
	
	try {
		auto reading_start_time = std::chrono::high_resolution_clock::now();
		vocabzer::ExtractorFactory extractor_factory;

		// extract text from file
		auto extractor = extractor_factory.create_extractor(file_path);
		auto& text = extractor->get_text();

		std::cout << "Text size in bytes: " << text.size() << std::endl;

		auto reading_end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(reading_end_time - reading_start_time);
		std::cout << "Reading time: " << duration.count() << " milliseconds" << std::endl;
		
		auto convert_start_time = std::chrono::high_resolution_clock::now();

		// text parse
		vocabzer::TextConverter text_converter{ text };
		auto& words = text_converter.get_words();

		auto convert_end_time = std::chrono::high_resolution_clock::now();
		auto convert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(convert_end_time - convert_start_time);
		std::cout << "Convert time: " << convert_duration.count() << " milliseconds" << std::endl;

		// output to file
		std::ofstream output(file_path_out, std::ios::binary);

		for (auto it = words.rbegin(); it != words.rend(); ++it) {
		 	output << it->second << ';' << it->first << ';' << '\n';
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}