#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

void write_uint32(std::vector<uint8_t>& out, uint32_t v);
void write_uint64(std::vector<uint8_t>& out, uint64_t v);
uint32_t read_uint32(const std::vector<uint8_t>& in, size_t& pos);
uint64_t read_uint64(const std::vector<uint8_t>& in, size_t& pos);

void write_varint(std::vector<uint8_t>& out, uint64_t v);
uint64_t read_varint(const std::vector<uint8_t>& in, size_t& pos);
