#pragma once
#include "../consensus/block.h"
#include "../crypto/uint256.h"
#include "../db/blockstore.h"
#include "utxo.h"
#include "blockindex.h"
#include <unordered_map>
#include <vector>
#include <optional>
#include <string>

class Blockchain {
public:
    explicit Blockchain(const std::string& datadir);

    bool AddBlock(const Block& block);
    std::optional<BlockIndexEntry> GetTip() const;
    std::optional<BlockIndexEntry> GetBlockIndex(const uint256& hash) const;
    uint32_t Height() const;

    // persistence of block index
    bool LoadIndex();
    bool SaveIndex();

    // accessors for other modules (safe references)
    BlockStore& GetBlockStore();
    UTXOSet& GetUTXO();

private:
    std::unordered_map<std::string, BlockIndexEntry> index_; // key = hash hex
    BlockStore store_;
    UTXOSet utxo_;
    uint32_t height_;
    std::string datadir_;
    void IndexBlock(const Block& block, const uint256& hash);
};
