#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include <fstream>
#include <sstream>
#include <iostream>

int main() {
    std::ifstream in("OYBuildfile");
    if (!in.is_open()) { std::cerr << "OYBuildfile not found\n"; return 1; }
    std::stringstream buffer; buffer << in.rdbuf();
    Lexer lexer(buffer.str());
    Parser parser(lexer);

    try {
        auto ast = parser.parse();
        for (auto stmt : ast) runStmt(stmt);
    } catch (std::exception& e) {
        std::cerr << "parse/exec error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
