#include "keystore.h"
#include "../crypto/sha256.h"
#include <filesystem>
#include <fstream>
#include <random>

std::string KeyStore::GenerateNew() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 255);
    std::vector<uint8_t> seed(32);
    for (auto& b : seed) b = static_cast<uint8_t>(dist(rd));

    const std::string priv = toHex(sha256(seed));
    std::vector<uint8_t> privBytes(priv.begin(), priv.end());
    const std::string pubkeyId = toHex(sha256(privBytes));
    keys_[pubkeyId] = priv;
    return pubkeyId;
}

std::vector<uint8_t> KeyStore::Sign(const std::string& msghex, const std::string& pubkeyid) {
    auto it = keys_.find(pubkeyid);
    if (it == keys_.end()) return {};

    std::vector<uint8_t> payload(msghex.begin(), msghex.end());
    payload.insert(payload.end(), it->second.begin(), it->second.end());
    auto sig = sha256(payload);
    return std::vector<uint8_t>(sig.begin(), sig.end());
}

bool KeyStore::Verify(const std::string& msghex, const std::string& pubkeyid, const std::vector<uint8_t>& signature) {
    if (signature.empty()) return false;
    const auto expected = Sign(msghex, pubkeyid);
    return !expected.empty() && expected == signature;
}

std::vector<std::string> KeyStore::ListKeys() const {
    std::vector<std::string> out;
    out.reserve(keys_.size());
    for (const auto& [key, _] : keys_) out.push_back(key);
    return out;
}

bool KeyStore::LoadFromFile(const std::string& path) {
    keys_.clear();
    std::ifstream in(path);
    if (!in.is_open()) return false;

    std::string pub, priv;
    while (in >> pub >> priv) keys_[pub] = priv;
    return true;
}

bool KeyStore::SaveToFile(const std::string& path) const {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& [pub, priv] : keys_) out << pub << ' ' << priv << '\n';
    return true;
}

bool KeyStore_LoadFromDefault(KeyStore& keystore, const std::string& datadir) {
    const std::string path = datadir + "/keystore.dat";
    if (keystore.LoadFromFile(path)) return true;
    return keystore.SaveToFile(path);
}
