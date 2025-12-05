#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../crypto/uint256.h"
#include "tx.h"

struct BlockHeader {
    uint32_t version = 1;
    uint256 prevBlock;
    uint256 merkleRoot;
    uint32_t time = 0;
    uint32_t bits = 0; // compact / prototype difficulty
    uint32_t nonce = 0;

    std::vector<uint8_t> serialize() const;
    static BlockHeader deserialize(const std::vector<uint8_t>& in, size_t& pos);
};

struct Block {
    BlockHeader header;
    std::vector<Tx> vtx;

    std::vector<uint8_t> serialize() const;
    static Block deserialize(const std::vector<uint8_t>& in, size_t& pos);
    uint256 headerHash() const;
};
