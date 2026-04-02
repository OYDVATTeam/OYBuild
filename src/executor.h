#ifndef BUILDSYS_EXECUTOR_H
#define BUILDSYS_EXECUTOR_H

#include "ast.h"
#include <map>
#include <string>
#include <vector>

extern std::map<std::string, std::vector<std::string>> env;

void runStmt(Stmt* stmt);
void compile(const std::string& file, const std::vector<std::string>& flags = {});
void cxxcompile(const std::string& file, const std::vector<std::string>& flags = {});
void link(const std::string& output, const std::vector<std::string>& objs);
void cxxlink(const std::string& output, const std::vector<std::string>& objs);
void static_lib(const std::string& output, const std::vector<std::string>& objs);
void cxxstatic_lib(const std::string& output, const std::vector<std::string>& objs);
void shared_lib(const std::string& output, const std::vector<std::string>& objs);
void cxxshared_lib(const std::string& output, const std::vector<std::string>& objs);
void command(const std::string& cmd);
void pkgConfig(const std::string& varName, const std::string& package);
void subdirectory(const std::string& path);

#endif
