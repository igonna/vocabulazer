#pragma once

#include <map>
#include <vector>
#include <string>

namespace vocabzer {

class TextConverter {
public:
	explicit TextConverter(const std::vector<char>& bytes);
	const std::multimap<uint32_t, std::string>& get_words() const noexcept;

private:
	uint32_t words_count = 0;
	std::multimap<uint32_t, std::string> stat_words;
};

}