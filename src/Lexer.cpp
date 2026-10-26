#include "Lexer.h"
#include "Exceptions.h"
#include <cctype>

Lexer::Lexer(std::string input) : m_input(std::move(input)) {}

char Lexer::peek() const {
    return (m_pos < m_input.size()) ? m_input[m_pos] : '\0';
}

char Lexer::get() {
    return (m_pos < m_input.size()) ? m_input[m_pos++] : '\0';
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) get();
}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;
    while (true) {
        skipWhitespace();
        char c = peek();
        if (c == '\0') {
            tokens.push_back({ 'E', "", 0 });
            break;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::string numStr;
            while (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
                numStr += get();
            }
            tokens.push_back({ 'N', numStr, std::stod(numStr) });
        }
        else if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string id;
            while (std::isalpha(static_cast<unsigned char>(peek()))) id += get();
            tokens.push_back({ 'I', id, 0 });
        }
        else {
            switch (c) {
            case '+': tokens.push_back({ '+', "+", 0 }); get(); break;
            case '-': tokens.push_back({ '-', "-", 0 }); get(); break;
            case '*': tokens.push_back({ '*', "*", 0 }); get(); break;
            case '/': tokens.push_back({ '/', "/", 0 }); get(); break;
            case '^': tokens.push_back({ '^', "^", 0 }); get(); break;
            case '(': tokens.push_back({ '(', "(", 0 }); get(); break;
            case ')': tokens.push_back({ ')', ")", 0 }); get(); break;
            case ',': tokens.push_back({ ',', ",", 0 }); get(); break;
            default:
                throw LexerException(std::string("Unexpected character: ") + c);
            }
        }
    }
    return tokens;
}