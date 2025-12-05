#include "logging.h"
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <ctime>

struct Logger::Impl {
    std::mutex m;
    std::unique_ptr<std::ofstream> out;
    bool inited = false;
};

Logger::Logger(): pimpl_(new Impl()) {}
Logger::~Logger() { if (pimpl_) delete pimpl_; }

static std::string NowTimestamp() {
    using namespace std::chrono;
    auto t = system_clock::now();
    std::time_t tt = system_clock::to_time_t(t);
    char buf[64];
#if defined(_MSC_VER)
    if (ctime_s(buf, sizeof(buf), &tt) != 0) return std::string();
#else
    std::tm tm;
    localtime_r(&tt, &tm);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
#endif
    std::string s(buf);
    // On MSVC ctime_s appends newline; remove it
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

bool Logger::Init(const std::string& path) {
    std::lock_guard<std::mutex> lk(pimpl_->m);
    pimpl_->out.reset(new std::ofstream(path, std::ios::app));
    pimpl_->inited = pimpl_->out->is_open();
    if (pimpl_->inited) {
        *pimpl_->out << "[" << NowTimestamp() << "] [INFO] Logger initialized\n";
        pimpl_->out->flush();
    }
    return pimpl_->inited;
}

void Logger::Info(const std::string& msg) {
    std::lock_guard<std::mutex> lk(pimpl_->m);
    if (pimpl_->inited) {
        *pimpl_->out << "[" << NowTimestamp() << "] [INFO] " << msg << "\n";
        pimpl_->out->flush();
    }
}

void Logger::Error(const std::string& msg) {
    std::lock_guard<std::mutex> lk(pimpl_->m);
    if (pimpl_->inited) {
        *pimpl_->out << "[" << NowTimestamp() << "] [ERROR] " << msg << "\n";
        pimpl_->out->flush();
    }
}