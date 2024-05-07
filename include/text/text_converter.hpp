#pragma once

#include <map>
#include <vector>
#include <string>
#include <unicode/unistr.h>

namespace vocabzer {

class TextConverter {
public:
	void parse_sentences(const std::vector<char>& bytes) noexcept;

	std::multimap<uint32_t, std::string>& get_words() noexcept;

	icu::UnicodeString get_text() noexcept;
	std::string get_text_utf() noexcept;

	TextConverter(std::string_view lang) noexcept;

private:
	uint32_t words_count = 0;
	std::multimap<uint32_t, std::string> stat_words;
	icu::UnicodeString clean_text;

	// array of sentences + it's tokens
	std::vector<std::tuple<icu::UnicodeString, std::vector<icu::UnicodeString>>> sentences;

	std::string lang;
};

}