#include "sha256.h"
#include <sstream>
#include <iomanip>
#ifdef USE_OPENSSL
#include <openssl/sha.h>
#endif

static inline uint8_t hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return 0;
}

std::array<uint8_t,32> sha256(const std::vector<uint8_t>& data) {
    std::array<uint8_t,32> out{};
#ifdef USE_OPENSSL
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, data.data(), data.size());
    SHA256_Final(out.data(), &ctx);
#else
    // Non-secure fallback for prototyping only
    uint8_t acc = 0;
    for (auto b : data) acc = (uint8_t)((acc * 131) ^ b);
    for (size_t i = 0; i < out.size(); ++i) out[i] = (uint8_t)(acc ^ (i * 31));
#endif
    return out;
}

std::array<uint8_t,32> sha256d(const std::vector<uint8_t>& data) {
    auto first = sha256(data);
    std::vector<uint8_t> tmp(first.begin(), first.end());
    return sha256(tmp);
}

std::string toHex(const std::array<uint8_t,32>& a) {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (auto b : a) ss << std::setw(2) << (int)b;
    return ss.str();
}

std::vector<uint8_t> fromHex(const std::string& hex) {
    std::vector<uint8_t> out;
    out.reserve((hex.size()+1)/2);
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        uint8_t hi = hexval(hex[i]);
        uint8_t lo = hexval(hex[i+1]);
        out.push_back((uint8_t)((hi<<4) | lo));
    }
    return out;
}
