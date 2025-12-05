#pragma once
#include <string>

struct Config {
    std::string data_dir;
    std::string log_file;

    static Config Default();
    bool LoadFromFile(const std::string& path);
};