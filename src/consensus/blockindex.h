#pragma once
#include "block.h"
#include "../crypto/uint256.h"
#include <cstdint>
#include <vector>

struct BlockIndexEntry {
    uint256 hash;
    BlockHeader header;
    uint32_t height = 0;

    std::vector<uint8_t> Serialize() const;
    static BlockIndexEntry Deserialize(const std::vector<uint8_t>& in);
};
