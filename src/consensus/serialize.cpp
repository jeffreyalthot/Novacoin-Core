#include "serialize.h"
#include <stdexcept>

void write_uint32(std::vector<uint8_t>& out, uint32_t v) {
    out.push_back(static_cast<uint8_t>(v & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
}

void write_uint64(std::vector<uint8_t>& out, uint64_t v) {
    for (int i = 0; i < 8; ++i) out.push_back(static_cast<uint8_t>((v >> (8 * i)) & 0xFF));
}

uint32_t read_uint32(const std::vector<uint8_t>& in, size_t& pos) {
    if (pos + 4 > in.size()) throw std::runtime_error("read_uint32 overflow");
    uint32_t v = static_cast<uint32_t>(in[pos]) |
                 (static_cast<uint32_t>(in[pos + 1]) << 8) |
                 (static_cast<uint32_t>(in[pos + 2]) << 16) |
                 (static_cast<uint32_t>(in[pos + 3]) << 24);
    pos += 4;
    return v;
}

uint64_t read_uint64(const std::vector<uint8_t>& in, size_t& pos) {
    if (pos + 8 > in.size()) throw std::runtime_error("read_uint64 overflow");
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v |= (static_cast<uint64_t>(in[pos + i]) << (8 * i));
    pos += 8;
    return v;
}

void write_varint(std::vector<uint8_t>& out, uint64_t v) {
    while (v >= 0x80) {
        out.push_back(static_cast<uint8_t>(v) | 0x80);
        v >>= 7;
    }
    out.push_back(static_cast<uint8_t>(v));
}

uint64_t read_varint(const std::vector<uint8_t>& in, size_t& pos) {
    uint64_t result = 0;
    int shift = 0;
    while (true) {
        if (pos >= in.size()) throw std::runtime_error("read_varint overflow");
        uint8_t b = in[pos++];
        result |= static_cast<uint64_t>(b & 0x7F) << shift;
        if ((b & 0x80) == 0) break;
        shift += 7;
        if (shift > 63) throw std::runtime_error("read_varint too large");
    }
    return result;
}
