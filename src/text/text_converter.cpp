#include "text/text_converter.hpp"

#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <unicode/uchar.h>
#include <unicode/unistr.h>

vocabzer::TextConverter::TextConverter(const std::vector<char>& bytes) {

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

    std::string str;
    clean_string.toUTF8String(str);

    // split array on words
    std::vector<std::string> words;
    boost::split(words, str, boost::is_any_of(" \n"), boost::token_compress_on);

    std::unordered_map<std::string, uint32_t> word_and_frequeuency;

    // count word
    for (auto& w : words) {
        word_and_frequeuency[w] += 1;
    }

    // sort word
    for (auto& count : word_and_frequeuency) {
        stat_words.insert({count.second, count.first});
    }
}

const std::multimap<uint32_t, std::string>& vocabzer::TextConverter::get_words() const noexcept {
    return stat_words;
}