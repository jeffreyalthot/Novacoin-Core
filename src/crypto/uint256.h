#pragma once
#include <array>
#include <cstdint>
#include <string>

struct uint256 {
    std::array<uint8_t, 32> data{};

    static uint256 fromHex(const std::string& hex);
    static uint256 fromBytes(const std::array<uint8_t, 32>& bytes);

    std::string toHex() const;

    bool operator==(const uint256& other) const;
    bool operator!=(const uint256& other) const;
};
