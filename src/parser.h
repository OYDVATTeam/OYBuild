#ifndef BUILDSYS_PARSER_H
#define BUILDSYS_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>

class Parser {
public:
    Parser(Lexer& lexer);
    std::vector<Stmt*> parse();

private:
    Lexer& lexer;
    Token current;

    Token consume(TokenType type, const std::string& msg);
    Stmt* parseStatement();
    Stmt* parseAssignment();
    Stmt* parseCallStmt();
    Stmt* parseFor();
    Expr* parseExpression();
    Expr* parseList();
};

#endif
