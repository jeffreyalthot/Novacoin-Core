#include "novacoin.h"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Novacoin-Core (minimal) - Windows build\n";
    Novacoin node;
    if(!node.Init()) {
        std::cerr << "Failed to initialize Novacoin node\n";
        return 1;
    }
    node.Start();
    std::cout << "Node started. Press Enter to stop...\n";
    std::cin.get();
    node.Stop();
    return 0;
}