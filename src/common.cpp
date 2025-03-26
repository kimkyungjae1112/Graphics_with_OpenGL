#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string& Filename) 
{
    std::ifstream Fin(Filename);
    if (!Fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", Filename);
        return {};
    }
    std::stringstream Text;
    Text << Fin.rdbuf();
    return Text.str();
}