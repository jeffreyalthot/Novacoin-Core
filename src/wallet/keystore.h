#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class KeyStore {
public:
    KeyStore() = default;
    std::string GenerateNew();
    std::vector<uint8_t> Sign(const std::string& msghex, const std::string& pubkeyid);
    std::vector<std::string> ListKeys() const;
private:
    std::unordered_map<std::string, std::string> keys_;
};
