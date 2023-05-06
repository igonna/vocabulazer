#include <iostream>
#include "file/text_extractor.hpp"

int main()
{
	std::filesystem::path file_path = "";
	
	try {
		vocabzer::TextExtractor extractor{file_path};
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}