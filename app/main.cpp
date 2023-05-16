#include <iostream>
#include <fstream>
#include <filesystem>
#include "file/extractor_factory.hpp"

int main(int argc, char* argv[])
{
	if (argc != 3) return 1;

	std::filesystem::path file_path{ argv[1] };
	std::filesystem::path file_path_out{ argv[2] };
	
	auto startTime = std::chrono::high_resolution_clock::now();
	try {
		vocabzer::ExtractorFactory extractor_factory;
		auto extractor = extractor_factory.create_extractor(file_path);

		auto& text = extractor->get_text();
		std::cout << "text size in bytes: " << text.size() << std::endl;

		// output result
		std::ofstream output(file_path_out, std::ios::binary);

		for (const auto& c : text)
			output << c;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

	
	return 0;
}