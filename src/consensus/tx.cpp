#include "tx.h"
#include "serialize.h"
#include "../crypto/sha256.h"
#include <stdexcept>

std::vector<uint8_t> Tx::serialize() const {
    std::vector<uint8_t> out;
    write_uint32(out, version);
    write_varint(out, vin.size());
    for (const auto& in : vin) {
        write_varint(out, in.prev.txid.size());
        out.insert(out.end(), in.prev.txid.begin(), in.prev.txid.end());
        write_uint32(out, in.prev.index);
        write_varint(out, in.scriptSig.size());
        out.insert(out.end(), in.scriptSig.begin(), in.scriptSig.end());
        write_uint32(out, in.sequence);
    }

    write_varint(out, vout.size());
    for (const auto& o : vout) {
        write_uint64(out, o.value);
        write_varint(out, o.scriptPubKey.size());
        out.insert(out.end(), o.scriptPubKey.begin(), o.scriptPubKey.end());
    }

    write_uint32(out, lockTime);
    return out;
}

Tx Tx::deserialize(const std::vector<uint8_t>& in, size_t& pos) {
    Tx tx;
    tx.version = read_uint32(in, pos);

    uint64_t inCount = read_varint(in, pos);
    for (uint64_t i = 0; i < inCount; ++i) {
        TxIn tin;
        uint64_t txidLen = read_varint(in, pos);
        if (pos + txidLen > in.size()) throw std::runtime_error("Tx::deserialize txid overflow");
        tin.prev.txid.assign(reinterpret_cast<const char*>(&in[pos]), txidLen);
        pos += txidLen;
        tin.prev.index = read_uint32(in, pos);
        uint64_t sigLen = read_varint(in, pos);
        if (pos + sigLen > in.size()) throw std::runtime_error("Tx::deserialize scriptSig overflow");
        tin.scriptSig.assign(in.begin() + static_cast<long>(pos), in.begin() + static_cast<long>(pos + sigLen));
        pos += sigLen;
        tin.sequence = read_uint32(in, pos);
        tx.vin.push_back(std::move(tin));
    }

    uint64_t outCount = read_varint(in, pos);
    for (uint64_t i = 0; i < outCount; ++i) {
        TxOut tout;
        tout.value = read_uint64(in, pos);
        uint64_t spkLen = read_varint(in, pos);
        if (pos + spkLen > in.size()) throw std::runtime_error("Tx::deserialize scriptPubKey overflow");
        tout.scriptPubKey.assign(in.begin() + static_cast<long>(pos), in.begin() + static_cast<long>(pos + spkLen));
        pos += spkLen;
        tx.vout.push_back(std::move(tout));
    }

    tx.lockTime = read_uint32(in, pos);
    return tx;
}

std::string Tx::txid() const {
    return toHex(sha256d(serialize()));
}
