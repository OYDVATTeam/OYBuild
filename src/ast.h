#ifndef BUILDSYS_AST_H
#define BUILDSYS_AST_H

#include <string>
#include <vector>

struct Expr { virtual ~Expr() = default; };
struct Stmt { virtual ~Stmt() = default; };

struct StringExpr : Expr {
    std::string value;
    StringExpr(std::string v) : value(v) {}
};

struct IdentifierExpr : Expr {
    std::string name;
    IdentifierExpr(std::string n) : name(n) {}
};

struct ListExpr : Expr {
    std::vector<Expr*> items;
    ListExpr(std::vector<Expr*> i) : items(i) {}
};

struct CallExpr : Expr {
    std::string callee;
    std::vector<Expr*> args;
    CallExpr(std::string c, std::vector<Expr*> a) : callee(c), args(a) {}
};

struct AssignmentStmt : Stmt {
    std::string name;
    Expr* value;
    AssignmentStmt(std::string n, Expr* v) : name(n), value(v) {}
};

struct CallStmt : Stmt {
    CallExpr* call;
    CallStmt(CallExpr* c) : call(c) {}
};

struct ForStmt : Stmt {
    std::string var;
    std::string iterable;
    std::vector<Stmt*> body;
    ForStmt(std::string v, std::string i, std::vector<Stmt*> b) 
        : var(v), iterable(i), body(b) {}
};

#endif
