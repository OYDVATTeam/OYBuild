#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    current = lexer.next();
}

Token Parser::consume(TokenType type, const std::string& msg) {
    if (current.type != type) throw std::runtime_error("Expected " + msg);
    Token old = current;
    current = lexer.next();
    return old;
}

std::vector<Stmt*> Parser::parse() {
    std::vector<Stmt*> program;
    while (current.type != TokenType::END_OF_FILE) {
        // Skip empty tokens if any
        program.push_back(parseStatement());
    }
    return program;
}

Stmt* Parser::parseStatement() {
    if (current.type == TokenType::FOR) return parseFor();
    
    // Assignment check: ID = ...
    if (current.type == TokenType::IDENTIFIER && lexer.peek().type == TokenType::EQUALS) {
        return parseAssignment();
    }
    
    return parseCallStmt();
}

Stmt* Parser::parseAssignment() {
    std::string name = consume(TokenType::IDENTIFIER, "identifier").text;
    consume(TokenType::EQUALS, "=");
    Expr* val = parseExpression();
    return new AssignmentStmt(name, val);
}

Stmt* Parser::parseCallStmt() {
    std::string callee = consume(TokenType::IDENTIFIER, "function name").text;
    consume(TokenType::LPAREN, "(");
    std::vector<Expr*> args;
    if (current.type != TokenType::RPAREN) {
        args.push_back(parseExpression());
        while (current.type == TokenType::COMMA) {
            consume(TokenType::COMMA, ",");
            args.push_back(parseExpression());
        }
    }
    consume(TokenType::RPAREN, ")");
    return new CallStmt(new CallExpr(callee, args));
}

Expr* Parser::parseExpression() {
    if (current.type == TokenType::LBRACKET) return parseList();
    
    if (current.type == TokenType::STRING) {
        std::string val = consume(TokenType::STRING, "string").text;
        return new StringExpr(val);
    }
    
    if (current.type == TokenType::IDENTIFIER) {
        std::string name = consume(TokenType::IDENTIFIER, "identifier").text;
        return new IdentifierExpr(name);
    }
    
    throw std::runtime_error("Unexpected token in expression: " + current.text);
}

Expr* Parser::parseList() {
    consume(TokenType::LBRACKET, "[");
    std::vector<Expr*> items;
    if (current.type != TokenType::RBRACKET) {
        items.push_back(parseExpression());
        while (current.type == TokenType::COMMA) {
            consume(TokenType::COMMA, ",");
            items.push_back(parseExpression());
        }
    }
    consume(TokenType::RBRACKET, "]");
    return new ListExpr(items);
}

Stmt* Parser::parseFor() {
    consume(TokenType::FOR, "for");
    consume(TokenType::LPAREN, "(");
    std::string var = consume(TokenType::IDENTIFIER, "loop variable").text;
    consume(TokenType::IN, "in");
    std::string iterable = consume(TokenType::IDENTIFIER, "list name").text;
    consume(TokenType::RPAREN, ")");
    consume(TokenType::DO, "do");

    std::vector<Stmt*> body;
    while (current.type != TokenType::ENDFOR && current.type != TokenType::END_OF_FILE) {
        body.push_back(parseStatement());
    }
    consume(TokenType::ENDFOR, "endfor");
    return new ForStmt(var, iterable, body);
}
