#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <vector>
#include <stdexcept>

namespace Tokenizer {
    using namespace Tokenizer;

    namespace {
        bool isChar(char test) {
            return ('a' <= test && test <= 'z') || ('A' <= test && test <= 'Z') || ('0' <= test && test <= '9');
        }

        bool isBracket(char c) {
            return c == '(' || c == ')' || c == ']' || c == '[';
        }

        bool isSymbol(char c) {
            return c == '*' || c == '+' || c == '?';
        }
    };

    enum Type {
        CHARACTER,
        BRACKET,
        SYMBOL,
        HYPHEN,
        PIPE,
    };

    struct Token {
        char character;
        Type type;

        Token(char character, Type type): character(character), type(type) {}
    };

    std::vector<Token> tokenize(std::string pattern) {
        std::vector<Token> tokens;
        bool tochar = false;

        for (char c: pattern) {
            if (tochar) {
                tokens.push_back(Token(c, CHARACTER));
                tochar = false;
            } else if (c == '\\') {
                tochar = true;
            } else if (isChar(c)) {
                tokens.push_back(Token(c, CHARACTER));
            } else if (isBracket(c)) {
                tokens.push_back(Token(c, BRACKET));
            } else if (isSymbol(c)) {
                tokens.push_back(Token(c, SYMBOL));
            } else if (c == '-') {
                tokens.push_back(Token(c, HYPHEN));
            } else if (c == '|') {
                tokens.push_back(Token(c, PIPE));
            } else {
                char message[20];
                snprintf(message, 20, "Invalid token '%c'", c);
                throw std::runtime_error(message);
            }
        }

        return tokens;
    }
};

#endif