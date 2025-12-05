#include "blockchain.h"
#include "validation.h"
#include <filesystem>
#include <fstream>
#include <iostream>

static std::string hashToHex(const uint256& h) { return h.toHex(); }

Blockchain::Blockchain(const std::string& datadir)
: store_(datadir), utxo_(datadir), height_(0), datadir_(datadir)
{
    utxo_.Load();
    if (!LoadIndex()) {
        std::cout << "Blockchain: no index present, starting fresh\n";
    } else {
        std::cout << "Blockchain: loaded index, height=" << height_ << "\n";
    }
}

bool Blockchain::LoadIndex() {
    std::string path = datadir_ + "/index.dat";
    if (!std::filesystem::exists(path)) return false;
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) return false;
    index_.clear();
    uint64_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint64_t i=0;i<count;i++) {
        uint32_t keylen = 0;
        in.read(reinterpret_cast<char*>(&keylen), sizeof(keylen));
        std::string key;
        key.resize(keylen);
        in.read(&key[0], keylen);
        uint32_t datalen = 0;
        in.read(reinterpret_cast<char*>(&datalen), sizeof(datalen));
        std::vector<uint8_t> data(datalen);
        in.read(reinterpret_cast<char*>(data.data()), datalen);
        try {
            auto entry = BlockIndexEntry::Deserialize(data);
            index_[key] = entry;
            if ((uint32_t)entry.height > height_) height_ = entry.height;
        } catch (const std::exception& e) {
            std::cerr << "LoadIndex: failed to deserialize entry: " << e.what() << "\n";
        }
    }
    return true;
}

bool Blockchain::SaveIndex() {
    std::string path = datadir_ + "/index.dat";
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return false;
    uint64_t count = index_.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& kv : index_) {
        const auto& key = kv.first;
        const auto& entry = kv.second;
        auto data = entry.Serialize();
        uint32_t keylen = (uint32_t)key.size();
        uint32_t datalen = (uint32_t)data.size();
        out.write(reinterpret_cast<const char*>(&keylen), sizeof(keylen));
        out.write(key.data(), keylen);
        out.write(reinterpret_cast<const char*>(&datalen), sizeof(datalen));
        if (datalen) out.write(reinterpret_cast<const char*>(data.data()), datalen);
    }
    return true;
}

void Blockchain::IndexBlock(const Block& block, const uint256& hash) {
    BlockIndexEntry bi;
    bi.hash = hash;
    bi.header = block.header;
    bi.height = height_ + 1;
    index_[hash.toHex()] = bi;
    height_ = bi.height;
}

bool Blockchain::AddBlock(const Block& block) {
    if (!ValidateBlockBasic(block, utxo_, false)) {
        std::cerr << "Blockchain::AddBlock: basic validation failed\n";
        return false;
    }
    auto hash = block.headerHash();
    if (!store_.WriteBlock(block, hash)) {
        std::cerr << "Blockchain::AddBlock: failed to write block to store\n";
        return false;
    }
    for (const auto& tx : block.vtx) {
        if (!ApplyTransactionToUTXO(tx, utxo_)) {
            std::cerr << "Blockchain::AddBlock: failed to apply tx to utxo\n";
            return false;
        }
    }
    if (!utxo_.Save()) {
        std::cerr << "Blockchain::AddBlock: failed to save utxo\n";
    }
    IndexBlock(block, hash);
    if (!SaveIndex()) {
        std::cerr << "Blockchain::AddBlock: warning failed to persist index\n";
    }
    std::cout << "Blockchain::AddBlock: accepted block " << hash.toHex() << " at height " << height_ << "\n";
    return true;
}

std::optional<BlockIndexEntry> Blockchain::GetTip() const {
    if (height_ == 0) return std::nullopt;
    for (const auto& kv : index_) {
        if (kv.second.height == height_) return kv.second;
    }
    return std::nullopt;
}

std::optional<BlockIndexEntry> Blockchain::GetBlockIndex(const uint256& hash) const {
    auto it = index_.find(hash.toHex());
    if (it == index_.end()) return std::nullopt;
    return it->second;
}

uint32_t Blockchain::Height() const { return height_; }

BlockStore& Blockchain::GetBlockStore() { return store_; }
UTXOSet& Blockchain::GetUTXO() { return utxo_; }
