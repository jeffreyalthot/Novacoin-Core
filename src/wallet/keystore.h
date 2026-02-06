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
    bool Verify(const std::string& msghex, const std::string& pubkeyid, const std::vector<uint8_t>& signature);
    std::vector<std::string> ListKeys() const;
    bool LoadFromFile(const std::string& path);
    bool SaveToFile(const std::string& path) const;
private:
    std::unordered_map<std::string, std::string> keys_;
};

bool KeyStore_LoadFromDefault(KeyStore& keystore, const std::string& datadir);
