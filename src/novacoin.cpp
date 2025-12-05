#include "novacoin.h"
#include <iostream>
#include <chrono>
#include <filesystem>

Novacoin::Novacoin(): running_(false) {}
Novacoin::~Novacoin() { Stop(); }

bool Novacoin::Init(const Config& cfg) {
    cfg_ = cfg;
    try {
        std::filesystem::create_directories(cfg_.data_dir);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create data directory: " << cfg_.data_dir << " (" << e.what() << ")\n";
        return false;
    } catch (...) {
        std::cerr << "Failed to create data directory: " << cfg_.data_dir << "\n";
        return false;
    }
    if (!logger_.Init(cfg_.log_file)) {
        std::cerr << "Warning: failed to open log file: " << cfg_.log_file << "\n";
    }
    logger_.Info("Initializing Novacoin node...");
    // Placeholder: load config, create data directory, init networking, etc.
    return true;
}

void Novacoin::Start() {
    if (running_) return;
    running_ = true;
    logger_.Info("Starting Novacoin node (mock)");
    worker_ = std::thread([this](){
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            logger_.Info("heartbeat");
        }
    });
}

void Novacoin::Stop() {
    if (!running_) return;
    running_ = false;
    if (worker_.joinable()) worker_.join();
    logger_.Info("Novacoin node stopped.");
}

std::string Novacoin::Version() const { return "0.1.0-windows-mock"; }