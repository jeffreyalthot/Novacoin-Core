#include "uint256.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

static uint8_t HexValue(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(10 + c - 'a');
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(10 + c - 'A');
    throw std::runtime_error("uint256: invalid hex character");
}

uint256 uint256::fromHex(const std::string& hex) {
    if (hex.size() != 64) throw std::runtime_error("uint256: expected 64-char hex string");
    uint256 out;
    for (size_t i = 0; i < 32; ++i) {
        out.data[i] = static_cast<uint8_t>((HexValue(hex[2 * i]) << 4) | HexValue(hex[2 * i + 1]));
    }
    return out;
}

uint256 uint256::fromBytes(const std::array<uint8_t, 32>& bytes) {
    uint256 out;
    out.data = bytes;
    return out;
}

std::string uint256::toHex() const {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (auto b : data) ss << std::setw(2) << static_cast<int>(b);
    return ss.str();
}

bool uint256::operator==(const uint256& other) const { return data == other.data; }
bool uint256::operator!=(const uint256& other) const { return !(*this == other); }
