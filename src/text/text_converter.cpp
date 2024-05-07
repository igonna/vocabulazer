#include "text/text_converter.hpp"

#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <utility>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <set>
#include <unordered_set>

static const std::unordered_map<std::string_view, std::unordered_set<UChar32>> one_len_words_per_lang = {
    {"en", {U'a', U'i', U'o'}},
    {"es", {U'y', U'o', U'u'}},
    {"it", {U'a', U'e', U'o'}},
    {"pt", {U'a', U'e', U'o'}},
    {"fr", {U'à', U'c', U'ù'}},
    {"de", {U'a', U'u', U'o'}},
    {"ru", {U'и', U'а', U'у'}}
};

vocabzer::TextConverter::TextConverter(std::string_view lang) noexcept {
    this->lang = lang;
}

icu::UnicodeString str_from_words(std::vector<icu::UnicodeString> words) {
    icu::UnicodeString result;
    for (auto& word : words) {
        result += word + ',';
    }
    return '[' + result + ']';
}

void vocabzer::TextConverter::parse_sentences(const std::vector<char>& bytes) noexcept {
    std::string str;
    
    {
        icu::UnicodeString unicode_string = icu::UnicodeString::fromUTF8(icu::StringPiece(bytes.data()));

        // cleaning string
        UChar32 new_line = U'\n';
        UChar32 whitespace = U' ';
        UChar32 double_quote = U'\"';
        UChar32 double_quote_1 = U'“';
        // UChar32 single_quote = U'\'';

        std::unordered_set<UChar32> single_quote{U'\'', U'’'};

        UChar32 space = U' ';
        UChar32 period = U'.';
        UChar32 question_mark = U'?';
        UChar32 exclamation_point = U'!';

        std::unordered_set<UChar32> ending_chars{U'.',  U'?', U'!'};


        



        
        std::unordered_set<UChar32> chars_to_not_skip{
            U' ', 
            U'\'', 
            U'’',
            // U'\"', 
            // U'“',
            U'\n',
            // U'!',
            // U'?',
        };
                
        std::unordered_set<UChar32> chars_to_include{
            U'!',
            U'?',
        };

        UChar32 hyphen = U'-';

        UChar32 c;

        int32_t min_sentence_len = 35;

        icu::UnicodeString current_phrase = "";
        int32_t current_phrase_len = 0;
        int32_t current_word_len = 0;
        std::vector<icu::UnicodeString> current_words;
        int32_t current_words_size = 0;

        bool is_alpha;
        icu::UnicodeString current_word;

        const int32_t min_words_per_phrase = 3;

        int32_t total_len = unicode_string.length();
        for (int32_t i = 0; i < total_len; ++i) {
            c = unicode_string.char32At(i);

            is_alpha = u_isalpha(c);

            if (single_quote.contains(c) && i + 1 <= total_len) {
                UChar32 future_char = unicode_string.char32At(i + 1);
                if (!u_isalpha(future_char)) continue;
            }

            if (is_alpha || single_quote.contains(c)) {
                if (single_quote.contains(c)) {
                    if (current_word_len == 0) continue;
                }
                current_word += u_tolower(c);
                ++current_word_len;
            } else {
                if (current_word_len != 0) {
                    current_words.push_back(current_word);
                    ++current_words_size;
                    current_word = "";
                    current_word_len = 0;
                }
            }

            if (is_alpha || chars_to_not_skip.contains(c)) {
                if (c == new_line || c == space) {
                    if (current_phrase_len == 0) continue;
                    if (current_phrase[current_phrase_len - 1] == space) continue;

                    current_phrase += space;
                    ++current_phrase_len;
                } else {
                    current_phrase += c;
                    ++current_phrase_len;
                }

            } else {

                if (chars_to_include.contains(c)) {
                    current_phrase += c;
                    ++current_phrase_len;
                }

                if (current_phrase != "") {
                    if (current_words_size >= min_words_per_phrase) {
                        bool verified_words = true;
                        auto one_len_words = one_len_words_per_lang.find(lang);
                        if (one_len_words != one_len_words_per_lang.end()) {
                            for (auto& word : current_words) {
                                if (word.length() == 1) {
                                    if (!one_len_words->second.contains(word[0])) {
                                        verified_words = false;
                                        break;
                                    }
                                }
                            }
                        }
                        if (verified_words) {
                            icu::UnicodeString verified_phrase = u_toupper(current_phrase[0]) + current_phrase.tempSubString(1, current_phrase_len - 1);
                            clean_text += verified_phrase + ' ' + str_from_words(current_words) + new_line;
                        }
                    }
                }

                current_phrase = "";
                current_words.clear();
                current_phrase_len = 0;
                current_words_size = 0;
            }
        }
        unicode_string.remove();
    }
}

std::multimap<uint32_t, std::string>& vocabzer::TextConverter::get_words() noexcept {
    std::string str;
    clean_text.toUTF8String(str);

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

    return stat_words;
}

icu::UnicodeString vocabzer::TextConverter::get_text() noexcept {
    return clean_text;
}

std::string vocabzer::TextConverter::get_text_utf() noexcept {
    std::string str;
    clean_text.toUTF8String(str);
    return str;
}