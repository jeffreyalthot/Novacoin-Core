#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct OutPoint {
    std::string txid;
    uint32_t index = 0;
};

struct TxIn {
    OutPoint prev;
    std::vector<uint8_t> scriptSig;
    uint32_t sequence = 0xFFFFFFFF;
};

struct TxOut {
    uint64_t value = 0;
    std::vector<uint8_t> scriptPubKey;
};

struct Tx {
    uint32_t version = 1;
    std::vector<TxIn> vin;
    std::vector<TxOut> vout;
    uint32_t lockTime = 0;

    std::vector<uint8_t> serialize() const;
    static Tx deserialize(const std::vector<uint8_t>& in, size_t& pos);
    std::string txid() const;
};
