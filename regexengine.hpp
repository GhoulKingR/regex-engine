#ifndef _REGEXENGINE_HPP_
#define _REGEXENGINE_HPP_

#include <string>
#include <iostream>
#include <list>
#include <stack>
#include "tokenizer.hpp"

#define ERR_SIZE 1000

class RegexEngine {
    struct Pattern {
        std::string pattern;
        char modifier;
        
        Pattern(std::string pattern): pattern(pattern), modifier('\0') {}
        Pattern(std::string pattern, char modifier): pattern(pattern), modifier(modifier) {}
    };

    std::vector<Pattern> get_patterns(std::string pattern) {
        using namespace Tokenizer;

        auto tokens = tokenize(pattern);
        std::vector<Pattern> patterns;
        std::list<Token> token_queue;
        for (auto token: tokens) {
            if (token_queue.size() > 0 && token_queue.back().type == HYPHEN) {
                token_queue.pop_back();
                auto back = token_queue.back().character;
                for (char c = back + 1; c <= token.character; c++) {
                    token_queue.push_back(Token(c, CHARACTER));
                }
            } else if (token.type == CHARACTER || token.type == HYPHEN || token.type == PIPE) {
                token_queue.push_back(token);
            } else if (token.type == BRACKET) {
                if (token_queue.front().type != BRACKET) {
                    std::string new_pattern = "";
                    while (!token_queue.empty()) {
                        auto front = token_queue.front();
                        token_queue.pop_front();
                        new_pattern += front.character;
                    }

                    if (new_pattern.length() > 0) {
                        patterns.push_back(Pattern(new_pattern));
                        new_pattern = "";
                    }
                }

                token_queue.push_back(token);
            } else {
                if (token_queue.size() == 0) {
                    char message[ERR_SIZE];
                    snprintf(message, ERR_SIZE, "Invalid pattern: cannot consecutively process '%c' in '%s'", token.character, pattern.c_str());
                    throw std::runtime_error(message);
                } else if (token_queue.back().type == BRACKET) {
                    std::string new_pattern = "";
                    while (!token_queue.empty()) {
                        auto front = token_queue.front();
                        token_queue.pop_front();
                        new_pattern += front.character;
                    }

                    if (new_pattern.length() > 0) {
                        patterns.push_back(Pattern(new_pattern, token.character));
                    }
                } else if (token_queue.back().type == CHARACTER) {
                    std::string new_pattern = "";
                    while (token_queue.size() > 1) {
                        auto front = token_queue.front();
                        token_queue.pop_front();
                        new_pattern += front.character;
                    }

                    if (new_pattern.length() > 0) {
                        patterns.push_back(Pattern(new_pattern));
                    }
                
                    new_pattern = "";
                    auto front = token_queue.front();
                    token_queue.pop_front();
                    new_pattern += front.character;
                    patterns.push_back(Pattern(new_pattern, token.character));
                } else {
                    char message[ERR_SIZE];
                    snprintf(message, ERR_SIZE, "Invalid pattern: cannot consecutively process '%c' in '%s'", token.character, pattern.c_str());
                    throw std::runtime_error(message);
                }
            }
        }

        return patterns;
    }

    std::vector<std::string> split(std::string text, char by) {
        std::vector<std::string> splits;
        std::string currentstr = "";
        for (char c: text) {
            if (c != by) {
                currentstr += c;
            } else {
                splits.push_back(currentstr);
                currentstr = "";
            }
        }
        splits.push_back(currentstr);
        return splits;
    }

    struct ProcessedPattern {
        std::vector<std::string> patterns;
        char modifier;
    };

    std::vector<ProcessedPattern> process_patterns(std::vector<Pattern> patterns) {
        std::vector<ProcessedPattern> processed;
        for (auto pattern: patterns) {
            ProcessedPattern processed_pattern;
            processed_pattern.modifier = pattern.modifier;

            auto text = pattern.pattern;
            if (text[0] == '(') {
                auto str = text.substr(1, text.size()-2);
                processed_pattern.patterns = split(str, '|');
            } else if (text[0] == '[') {
                std::string str = text.substr(1, text.size()-2);
                for (char c: str) {
                    std::string str(1, c);
                    processed_pattern.patterns.push_back(str);
                }
            } else {
                processed_pattern.patterns.push_back(text);
            }

            processed.push_back(processed_pattern);
        }
        return processed;
    }

    std::vector<ProcessedPattern> processed;

    bool recursively_match(std::string with, int str_index, int current_pattern) {
        bool result = false;

        for (auto pattern: processed[current_pattern].patterns) {
            if (with.substr(str_index, pattern.length()) == pattern) {
                if (current_pattern == processed.size() - 1) {
                    return str_index + pattern.length() == with.length();
                } else {
                    result = result || recursively_match(with, pattern.length() + str_index, current_pattern + 1);
                }

                if (processed[current_pattern].modifier == '*' || processed[current_pattern].modifier == '+') {
                    result = result || recursively_match(with, str_index + pattern.length(), current_pattern);
                }
            }
        }

        if (processed[current_pattern].modifier == '*' || processed[current_pattern].modifier == '?') {
            result = result || recursively_match(with, str_index, current_pattern + 1);
        }

        return result;
    }

public:
    RegexEngine(std::string pattern) {
        auto patterns = get_patterns(pattern);
        processed = process_patterns(patterns);
    }

    bool match(std::string with) {
        return recursively_match(with, 0, 0);
    }
};

#endif