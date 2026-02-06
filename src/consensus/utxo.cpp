#include "utxo.h"
#include "serialize.h"
#include <filesystem>
#include <fstream>

UTXOSet::UTXOSet(const std::string& datadir) : path_(datadir + "/utxo.dat") {}

std::string UTXOSet::MakeKey(const std::string& txid, uint32_t idx) const {
    return txid + ":" + std::to_string(idx);
}

bool UTXOSet::Has(const std::string& txid, uint32_t idx) const {
    return map_.find(MakeKey(txid, idx)) != map_.end();
}

std::optional<UTXOEntry> UTXOSet::Get(const std::string& txid, uint32_t idx) const {
    auto it = map_.find(MakeKey(txid, idx));
    if (it == map_.end()) return std::nullopt;
    return it->second;
}

void UTXOSet::Add(const std::string& txid, uint32_t idx, const UTXOEntry& e) {
    map_[MakeKey(txid, idx)] = e;
}

bool UTXOSet::Spend(const std::string& txid, uint32_t idx) {
    return map_.erase(MakeKey(txid, idx)) > 0;
}

size_t UTXOSet::Size() const { return map_.size(); }

bool UTXOSet::Save() {
    std::filesystem::create_directories(std::filesystem::path(path_).parent_path());
    std::ofstream out(path_, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return false;

    std::vector<uint8_t> data;
    write_varint(data, map_.size());
    for (const auto& [key, utxo] : map_) {
        write_varint(data, key.size());
        data.insert(data.end(), key.begin(), key.end());
        write_uint64(data, utxo.value);
        write_varint(data, utxo.scriptPubKey.size());
        data.insert(data.end(), utxo.scriptPubKey.begin(), utxo.scriptPubKey.end());
    }

    out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    return true;
}

bool UTXOSet::Load() {
    map_.clear();
    if (!std::filesystem::exists(path_)) return false;

    std::ifstream in(path_, std::ios::binary);
    if (!in.is_open()) return false;

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    size_t pos = 0;
    uint64_t count = read_varint(data, pos);
    for (uint64_t i = 0; i < count; ++i) {
        uint64_t keyLen = read_varint(data, pos);
        if (pos + keyLen > data.size()) return false;
        std::string key(reinterpret_cast<const char*>(&data[pos]), keyLen);
        pos += keyLen;

        UTXOEntry entry;
        entry.value = read_uint64(data, pos);
        uint64_t scriptLen = read_varint(data, pos);
        if (pos + scriptLen > data.size()) return false;
        entry.scriptPubKey.assign(data.begin() + static_cast<long>(pos), data.begin() + static_cast<long>(pos + scriptLen));
        pos += scriptLen;

        map_[std::move(key)] = std::move(entry);
    }
    return true;
}
