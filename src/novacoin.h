#pragma once
#include "config.h"
#include "logging.h"
#include <string>
#include <thread>
#include <atomic>

class Novacoin {
public:
    Novacoin();
    ~Novacoin();
    bool Init(const Config& cfg);
    void Start();
    void Stop();
    std::string Version() const;
private:
    std::atomic<bool> running_;
    std::thread worker_;
    Config cfg_;
    Logger logger_;
};