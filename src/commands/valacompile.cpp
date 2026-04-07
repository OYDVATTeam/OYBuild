#include "valacompile.hpp"
#include <iostream>
#include <cstdlib>

void handle_valacompile(const std::string& source, const std::vector<std::string>& flags) {
    std::string cmd = "valac -c " + source;
    for (const auto& f : flags) {
        cmd += " " + f;
    }

    std::cout << "[valacompile] Compiling Vala source " << source << std::endl;
    if (std::system(cmd.c_str()) != 0) {
        throw std::runtime_error("Vala compilation failed.");
    }
}
