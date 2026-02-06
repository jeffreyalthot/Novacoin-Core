#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <filesystem>

static std::string Trim(const std::string& s) {
    size_t a = 0;
    while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    size_t b = s.size();
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b-a);
}

static std::string DefaultDataDir() {
#ifdef _WIN32
    const char* appdata = std::getenv("APPDATA");
    if (appdata && appdata[0] != '\0') {
        return (std::filesystem::path(appdata) / "Novacoin").string();
    }
    return "data";
#else
    const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
    if (xdgDataHome && xdgDataHome[0] != '\0') {
        return (std::filesystem::path(xdgDataHome) / "novacoin").string();
    }

    const char* home = std::getenv("HOME");
    if (home && home[0] != '\0') {
        return (std::filesystem::path(home) / ".novacoin").string();
    }

    return "data";
#endif
}

static std::string DefaultLogFile(const std::string& dataDir) {
    return (std::filesystem::path(dataDir) / "novacoin.log").string();
}

Config Config::Default() {
    Config c;
    c.data_dir = DefaultDataDir();
    c.log_file = DefaultLogFile(c.data_dir);
    return c;
}

bool Config::LoadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    bool dataDirSet = false;
    bool logFileSet = false;

    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find('#');
        if (pos != std::string::npos) line = line.substr(0, pos);
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = Trim(line.substr(0, eq));
        std::string val = Trim(line.substr(eq+1));

        if (key == "data_dir") {
            data_dir = val;
            dataDirSet = true;
        } else if (key == "log_file") {
            log_file = val;
            logFileSet = true;
        }
    }

    if (dataDirSet && !logFileSet) {
        log_file = DefaultLogFile(data_dir);
    }

    return true;
}
