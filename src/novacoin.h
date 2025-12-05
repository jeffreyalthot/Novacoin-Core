#pragma once
#include <string>
#include <thread>
#include <atomic>

class Novacoin {
public:
    Novacoin();
    ~Novacoin();
    bool Init();
    void Start();
    void Stop();
    std::string Version() const;
private:
    std::atomic<bool> running_;
    std::thread worker_;
};