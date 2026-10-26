#pragma once
#include <string>
#include <vector>

struct Token {
    char type;         
    std::string text;  
    double value = 0.0;
};

class Lexer {
public:
    explicit Lexer(std::string input);
    std::vector<Token> Tokenize();

private:
    std::string m_input;
    size_t m_pos = 0;

    char peek() const;
    char get();
    void skipWhitespace();
};