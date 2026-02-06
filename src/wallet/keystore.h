#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class KeyStore {
public:
    KeyStore() = default;
    std::string GenerateNew();
    std::vector<uint8_t> Sign(const std::string& msghex, const std::string& pubkeyid);
    std::vector<std::string> ListKeys() const;
    bool LoadFromFile(const std::string& path);
    bool SaveToFile(const std::string& path) const;
private:
    std::unordered_map<std::string, std::string> keys_;
};

bool KeyStore_LoadFromDefault(KeyStore& keystore, const std::string& datadir);
