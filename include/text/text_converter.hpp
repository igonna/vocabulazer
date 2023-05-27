#pragma once

#include <map>
#include <vector>
#include <string>

namespace vocabzer {

class TextConverter {
public:
	explicit TextConverter(const std::vector<char>& bytes);
	std::multimap<uint32_t, std::string>& get_words() noexcept;

private:
	uint32_t words_count = 0;
	std::multimap<uint32_t, std::string> stat_words;
};

}