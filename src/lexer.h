#ifndef BUILDSYS_LEXER_H
#define BUILDSYS_LEXER_H

#include <string>

enum class TokenType {
    IDENTIFIER, STRING, FOR, IN, DO, ENDFOR,
    LPAREN, RPAREN, LBRACKET, RBRACKET, EQUALS, COMMA,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    Lexer(const std::string& source);
    Token next();
    Token peek();

private:
    std::string src;
    size_t pos;
    std::string parseString();
};

#endif
