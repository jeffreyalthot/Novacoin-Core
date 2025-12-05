#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cctype>

static std::string Trim(const std::string& s) {
    size_t a = 0;
    while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    size_t b = s.size();
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b-a);
}

Config Config::Default() {
    Config c;
    const char* appdata = std::getenv("APPDATA");
    if (appdata && appdata[0] != '\0') {
        c.data_dir = std::string(appdata) + "\\Novacoin";
    } else {
        c.data_dir = "data";
    }
    c.log_file = c.data_dir + "\\novacoin.log";
    return c;
}

bool Config::LoadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return false;
    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find('#');
        if (pos != std::string::npos) line = line.substr(0, pos);
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = Trim(line.substr(0, eq));
        std::string val = Trim(line.substr(eq+1));
        if (key == "data_dir") data_dir = val;
        else if (key == "log_file") log_file = val;
    }
    return true;
}