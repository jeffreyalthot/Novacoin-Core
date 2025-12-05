#include "uint256.h"
#include <stdexcept>
#include <cctype>

uint256 uint256::fromHex(const std::string& hex) {
    if (hex.size() != 64) throw std::runtime_error("uint256::fromHex: invalid hex length");
    uint256 u;
    for (size_t i = 0; i < 32; ++i) {
        auto hi = hex[2*i];
        auto lo = hex[2*i+1];
        auto val = [](char c)->uint8_t {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
            if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
            throw std::runtime_error("uint256::fromHex: invalid char");
        };
        u.data[i] = (uint8_t)((val(hi) << 4) | val(lo));
    }
    return u;
}
