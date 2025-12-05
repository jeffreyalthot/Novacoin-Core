#pragma once
#include <array>
#include <vector>
#include <string>
#include <cstdint>

std::array<uint8_t,32> sha256(const std::vector<uint8_t>& data);
std::array<uint8_t,32> sha256d(const std::vector<uint8_t>& data);
std::string toHex(const std::array<uint8_t,32>& a);
std::vector<uint8_t> fromHex(const std::string& hex);
