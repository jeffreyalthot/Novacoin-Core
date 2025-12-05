#include "novacoin.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

int main(int argc, char** argv) {
    Config cfg = Config::Default();
    // Try to load local novacoin.conf if present or if passed as arg
    if (argc >= 2) {
        std::string cfgpath = argv[1];
        if (cfg.LoadFromFile(cfgpath)) {
            std::cout << "Loaded config from: " << cfgpath << "\n";
        } else {
            std::cout << "Failed to load config from: " << cfgpath << ", using defaults\n";
        }
    } else {
        if (std::ifstream("novacoin.conf").good()) {
            if (cfg.LoadFromFile("novacoin.conf")) std::cout << "Loaded config from novacoin.conf\n";
        }
    }

    std::cout << "Novacoin-Core (minimal) - Windows build\n";
    std::cout << "Data directory: " << cfg.data_dir << "\n";
    std::cout << "Log file: " << cfg.log_file << "\n";

    Novacoin node;
    if(!node.Init(cfg)) {
        std::cerr << "Failed to initialize Novacoin node\n";
        return 1;
    }
    node.Start();
    std::cout << "Node started. Press Enter to stop...\n";
    std::cin.get();
    node.Stop();
    return 0;
}