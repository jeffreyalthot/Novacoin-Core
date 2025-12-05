#pragma once
#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <unordered_map>
#include "../consensus/tx.h"

struct UTXOEntry {
    uint64_t value;
    std::vector<uint8_t> scriptPubKey;
};

class UTXOSet {
public:
    explicit UTXOSet(const std::string& datadir);
    bool Has(const std::string& txid, uint32_t idx) const;
    std::optional<UTXOEntry> Get(const std::string& txid, uint32_t idx) const;
    void Add(const std::string& txid, uint32_t idx, const UTXOEntry& e);
    bool Spend(const std::string& txid, uint32_t idx);
    size_t Size() const;
    bool Save();
    bool Load();
private:
    std::string MakeKey(const std::string& txid, uint32_t idx) const;
    std::unordered_map<std::string, UTXOEntry> map_;
    std::string path_;
};
