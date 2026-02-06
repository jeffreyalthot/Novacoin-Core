#include "merkle.h"
#include "../crypto/sha256.h"

uint256 merkleRootFromTxs(const std::vector<Tx>& txs) {
    if (txs.empty()) {
        return uint256{};
    }

    std::vector<std::array<uint8_t, 32>> level;
    level.reserve(txs.size());
    for (const auto& tx : txs) {
        level.push_back(sha256d(tx.serialize()));
    }

    while (level.size() > 1) {
        if ((level.size() & 1U) != 0U) level.push_back(level.back());
        std::vector<std::array<uint8_t, 32>> next;
        next.reserve(level.size() / 2);

        for (size_t i = 0; i < level.size(); i += 2) {
            std::vector<uint8_t> cat;
            cat.reserve(64);
            cat.insert(cat.end(), level[i].begin(), level[i].end());
            cat.insert(cat.end(), level[i + 1].begin(), level[i + 1].end());
            next.push_back(sha256d(cat));
        }
        level = std::move(next);
    }

    return uint256::fromBytes(level[0]);
}
