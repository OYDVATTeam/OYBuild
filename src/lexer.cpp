#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& source) : src(source), pos(0) {}

Token Lexer::peek() {
    size_t old_pos = pos;
    Token tok = next();
    pos = old_pos;
    return tok;
}

std::string Lexer::parseString() {
    std::string result;
    pos++;
    while (pos < src.size() && src[pos] != '"') {
        if (src[pos] == '\\' && pos + 1 < src.size()) pos++; 
        result += src[pos++];
    }
    if (pos >= src.size()) throw std::runtime_error("Unterminated string");
    pos++;
    return result;
}

Token Lexer::next() {
    while (pos < src.size() && isspace(src[pos])) pos++;
    if (pos >= src.size()) return {TokenType::END_OF_FILE, ""};

    if (src[pos] == '#') {
        while (pos < src.size() && src[pos] != '\n') pos++;
        return next();
    }

    if (src[pos] == '"') return {TokenType::STRING, parseString()};

    if (isalpha(src[pos]) || src[pos] == '_') {
        std::string s;
        while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_')) s += src[pos++];
        if (s == "for") return {TokenType::FOR, s};
        if (s == "in") return {TokenType::IN, s};
        if (s == "do") return {TokenType::DO, s};
        if (s == "endfor") return {TokenType::ENDFOR, s};
        return {TokenType::IDENTIFIER, s};
    }

    if (src[pos] == '(') { pos++; return {TokenType::LPAREN, "("}; }
    if (src[pos] == ')') { pos++; return {TokenType::RPAREN, ")"}; }
    if (src[pos] == '[') { pos++; return {TokenType::LBRACKET, "["}; }
    if (src[pos] == ']') { pos++; return {TokenType::RBRACKET, "]"}; }
    if (src[pos] == '=') { pos++; return {TokenType::EQUALS, "="}; }
    if (src[pos] == ',') { pos++; return {TokenType::COMMA, ","}; }

    throw std::runtime_error(std::string("unexpected char: ") + src[pos]);
}
