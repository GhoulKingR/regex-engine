#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <list>
#include <stdexcept>

#define ERR_SIZE 1000

namespace Tokenizer {
    using namespace Tokenizer;

    namespace {
        bool isChar(char test) {
            return ('a' <= test && test <= 'z') || ('A' <= test && test <= 'Z') || ('0' <= test && test <= '9');
        }

        bool isBoundary(char chr) {
            return chr == '^' || chr == '$';
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
        BOUNDARY,
        WILDCARD
    };

    struct Token {
        char character;
        Type type;

        Token(char character, Type type): character(character), type(type) {}
    };

    std::list<Token> tokenize(std::string& pattern) {
        std::list<Token> tokens;
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
            } else if (isBoundary(c)) {
                tokens.push_back(Token(c, BOUNDARY));
            } else if (c == '-') {
                tokens.push_back(Token(c, HYPHEN));
            } else if (c == '|') {
                tokens.push_back(Token(c, PIPE));
            } else if (c == '.') {
                tokens.push_back(Token(c, WILDCARD));
            } else {
                char message[ERR_SIZE];
                snprintf(message, ERR_SIZE, "Invalid token '%c' in \"%s\"", c, pattern);
                throw std::runtime_error(message);
            }
        }

        return tokens;
    }
};

#endif