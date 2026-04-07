#include "executor.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cstdio>
#include <memory>
#include <sstream>
#include <fstream>
#include <unistd.h>
#ifdef OYBUILD_COMPILING_WITH_BOOTSTRAPPED_OYBUILD
    #include "commands/valacompile.hpp"
#endif

std::map<std::string, std::vector<std::string>> env;

void runCommandOrFail(const std::string& cmd) {
    int ret = system(cmd.c_str());
    if (ret != 0) throw std::runtime_error("command failed: " + cmd);
}

std::string getCommandOutput(const std::string& cmd) {
    char buffer[128];
    std::string result = "";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) result += buffer;
    return result;
}

void pkgConfig(const std::string& varName, const std::string& package) {
    std::cout << "[pkg-config] " << package << "\n";
    std::string output = getCommandOutput("pkg-config --cflags --libs " + package);
    std::vector<std::string> flags;
    std::stringstream ss(output);
    std::string flag;
    while (ss >> flag) flags.push_back(flag);
    env[varName] = flags;
}

void subdirectory(const std::string& path) {
    char cwd[1024];
    if (!getcwd(cwd, sizeof(cwd))) throw std::runtime_error("failed to get cwd");

    std::cout << "[dir] entering " << path << "\n";
    if (chdir(path.c_str()) != 0) throw std::runtime_error("could not enter: " + path);

    std::ifstream in("OYBuildfile");
    if (!in.is_open()) {
        chdir(cwd);
        throw std::runtime_error("invalid subdirectory: " + path + " (missing OYBuildfile)");
    }

    std::stringstream buffer; buffer << in.rdbuf();
    Lexer subLexer(buffer.str());
    Parser subParser(subLexer);

    try {
        auto ast = subParser.parse();
        for (auto stmt : ast) runStmt(stmt);
    } catch (...) {
        chdir(cwd);
        throw;
    }
    chdir(cwd);
}

void compile(const std::string& file, const std::vector<std::string>& flags) {
    std::cout << "[compile] Compiling C file " << file << std::endl;
    std::string cmd = "gcc -c " + file;
    for (auto& f : flags) cmd += " " + f;
    runCommandOrFail(cmd);
}

void cxxcompile(const std::string& file, const std::vector<std::string>& flags) {
    std::cout << "[cxxcompile] Compiling C++ file " << file << std::endl;
    std::string cmd = "g++ -c " + file;
    for (auto& f : flags) cmd += " " + f;
    runCommandOrFail(cmd);
}

void link(const std::string& output, const std::vector<std::string>& args) {
    std::cout << "[link] Linking C executable " << output << std::endl;
    std::string cmd = "gcc -o " + output;
    for (auto& a : args) cmd += " " + a;
    runCommandOrFail(cmd);
}

void cxxlink(const std::string& output, const std::vector<std::string>& args) {
    std::cout << "[cxxlink] Linking C++ executable " << output << std::endl;
    std::string cmd = "g++ -o " + output;
    for (auto& a : args) cmd += " " + a;
    runCommandOrFail(cmd);
}

void static_lib(const std::string& output, const std::vector<std::string>& objs) {
    std::cout << "[static_lib] Creating static library " << output << std::endl;
    std::string cmd = "ar rcs " + output;
    for (auto& o : objs) cmd += " " + o;
    runCommandOrFail(cmd);
}

void cxxstatic_lib(const std::string& output, const std::vector<std::string>& objs) {
    static_lib(output, objs); // ar is language neutral, no need for static_lib copied as cxxstatic_lib
}

void shared_lib(const std::string& output, const std::vector<std::string>& objs) {
    std::cout << "[shared_lib] Linking C shared library " << output << std::endl;
    std::string cmd = "gcc -shared -o " + output;
    for (auto& o : objs) cmd += " " + o;
    runCommandOrFail(cmd);
}

void cxxshared_lib(const std::string& output, const std::vector<std::string>& objs) {
    std::cout << "[cxxlink] Linking CXX shared library " << output << std::endl;
    std::string cmd = "g++ -shared -o " + output;
    for (auto& o : objs) cmd += " " + o;
    runCommandOrFail(cmd);
}

void runStmt(Stmt* stmt) {
    if (auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        if (auto list = dynamic_cast<ListExpr*>(assign->value)) {
            std::vector<std::string> items;
            for (auto e : list->items) {
                if (auto s = dynamic_cast<StringExpr*>(e)) items.push_back(s->value);
                else if (auto id = dynamic_cast<IdentifierExpr*>(e)) {
                    auto it = env.find(id->name);
                    if (it != env.end()) items.insert(items.end(), it->second.begin(), it->second.end());
                }
            }
            env[assign->name] = items;
        }
    }
    else if (auto call = dynamic_cast<CallStmt*>(stmt)) {
        std::vector<std::string> args;
        for (auto e : call->call->args) {
            if (auto s = dynamic_cast<StringExpr*>(e)) args.push_back(s->value);
            else if (auto id = dynamic_cast<IdentifierExpr*>(e)) {
                auto it = env.find(id->name);
                if (it != env.end()) args.insert(args.end(), it->second.begin(), it->second.end());
            }
        }

        if (args.empty()) return;

        const std::string& callee = call->call->callee;
        std::vector<std::string> rest(args.begin() + 1, args.end());

        if (callee == "compile") compile(args[0], rest);
        else if (callee == "cxxcompile") cxxcompile(args[0], rest);
        else if (callee == "link") link(args[0], rest);
        else if (callee == "cxxlink") cxxlink(args[0], rest);
        else if (callee == "static_lib") static_lib(args[0], rest);
        else if (callee == "cxxstatic_lib") cxxstatic_lib(args[0], rest);
        else if (callee == "shared_lib") shared_lib(args[0], rest);
        else if (callee == "cxxshared_lib") cxxshared_lib(args[0], rest);
        else if (callee == "PkgConfig" && args.size() == 2) pkgConfig(args[0], args[1]);
        else if (callee == "subdirectory" && args.size() == 1) subdirectory(args[0]);
        else if (callee == "command" && args.size() == 1) runCommandOrFail(args[0]);
#ifdef OYBUILD_COMPILING_WITH_BOOTSTRAPPED_OYBUILD
        else if (callee == "valacompile" && args.size() >= 1) {
            // args[0] is the source file
            // rest contains all other flags/packages
            handle_valacompile(args[0], rest);
        }
#endif
    }
    else if (auto f = dynamic_cast<ForStmt*>(stmt)) {
        auto it = env.find(f->iterable);
        if (it == env.end()) throw std::runtime_error("variable not found: " + f->iterable);

        // Save previous value to restore after loop (simple scoping)
        std::vector<std::string> prev_val;
        bool existed = env.count(f->var);
        if (existed) prev_val = env[f->var];

        for (auto& val : it->second) {
            env[f->var] = {val}; 
            for (auto s : f->body) runStmt(s);
        }

        if (existed) env[f->var] = prev_val;
        else env.erase(f->var);
    }
}
