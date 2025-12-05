#include "miner.h"
#include "../crypto/sha256.h"
#include "../crypto/uint256.h"
#include "../consensus/merkle.h"
#include <iostream>
#include <chrono>

static bool CheckProofOfWork(const BlockHeader& h, uint32_t bits) {
    auto hash = sha256d(h.serialize());
    // simple difficulty: bits = number of leading zero bytes required
    uint32_t zeros = bits;
    for (uint32_t i = 0; i < zeros; ++i) {
        if (hash[i] != 0) return false;
    }
    return true;
}

int MineBlock(Block& block, BlockStore& store, int maxNonceTries) {
    std::cout << "Mining block with target zeros = " << block.header.bits << std::endl;
    for (uint32_t n = 0; n < (uint32_t)maxNonceTries; ++n) {
        block.header.nonce = n;
        block.header.time = (uint32_t)std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        if (CheckProofOfWork(block.header, block.header.bits)) {
            auto hash = block.headerHash();
            std::cout << "Mined block: " << hash.toHex() << " nonce=" << n << std::endl;
            store.WriteBlock(block, hash);
            return 0;
        }
        if ((n & 0xFFFF) == 0) {
            std::cout << "Tried nonce " << n << "...";
        }
    }
    std::cout << std::endl;
    std::cout << "No block found within nonce limit" << std::endl;
    return 1;
}
