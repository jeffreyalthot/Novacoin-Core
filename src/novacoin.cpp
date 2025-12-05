#include "novacoin.h"
#include <iostream>
#include <chrono>

Novacoin::Novacoin(): running_(false) {}
Novacoin::~Novacoin() { Stop(); }

bool Novacoin::Init() {
    std::cout << "Initializing Novacoin node...\n";
    // Placeholder: load config, create data directory, init networking, etc.
    return true;
}

void Novacoin::Start() {
    if (running_) return;
    running_ = true;
    std::cout << "Starting Novacoin node (mock) ...\n";
    worker_ = std::thread([this](){
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cout << "[novacoin] heartbeat\n";
        }
    });
}

void Novacoin::Stop() {
    if (!running_) return;
    running_ = false;
    if (worker_.joinable()) worker_.join();
    std::cout << "Novacoin node stopped.\n";
}

std::string Novacoin::Version() const { return "0.1.0-windows-mock"; }