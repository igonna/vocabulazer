#include "text/text_converter.hpp"

#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <utility>
#include <unicode/uchar.h>
#include <unicode/unistr.h>

vocabzer::TextConverter::TextConverter(const std::vector<char>& bytes) {

    std::string str;
    
    {
        icu::UnicodeString unicode_string = icu::UnicodeString::fromUTF8(icu::StringPiece(bytes.data()));

        icu::UnicodeString clean_string;

        // cleaning string
        UChar32 apostrophe = U'\'';
        UChar32 whitespace = U' ';
        UChar32 c;
        for (int32_t i = 0; i < unicode_string.length(); ++i) {
            c = unicode_string.char32At(i);
            if (u_isalpha(c) || c == whitespace || c == apostrophe) {
                clean_string += u_tolower(c);
            } else {
                clean_string += whitespace;
            }
        }
        unicode_string.remove();

        clean_string.toUTF8String(str);
    }

    std::unordered_map<std::string, uint32_t> word_and_frequeuency;

    // count words
    std::string tmp;
    for (char c : str) {
        if (c == ' ' || c == '\n') {
            if (tmp.size()) {
                word_and_frequeuency[std::move(tmp)] += 1;
                tmp.clear();
            }
        } else {
            tmp += c;
        }
    }

    // sort word
    for (auto& count : word_and_frequeuency) {
        stat_words.insert({count.second, count.first});
    }
}

std::multimap<uint32_t, std::string>& vocabzer::TextConverter::get_words() noexcept {
    return stat_words;
}