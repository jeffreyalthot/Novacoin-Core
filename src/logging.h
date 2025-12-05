#pragma once
#include <string>

class Logger {
public:
    Logger();
    ~Logger();
    bool Init(const std::string& path);
    void Info(const std::string& msg);
    void Error(const std::string& msg);
private:
    struct Impl;
    Impl* pimpl_;
};