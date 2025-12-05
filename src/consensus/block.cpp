#include "block.h"
#include "../crypto/sha256.h"
#include "../consensus/serialize.h"
#include <stdexcept>

std::vector<uint8_t> BlockHeader::serialize() const {
    std::vector<uint8_t> out;
    write_uint32(out, version);
    out.insert(out.end(), prevBlock.data.begin(), prevBlock.data.end());
    out.insert(out.end(), merkleRoot.data.begin(), merkleRoot.data.end());
    write_uint32(out, time);
    write_uint32(out, bits);
    write_uint32(out, nonce);
    return out;
}

BlockHeader BlockHeader::deserialize(const std::vector<uint8_t>& in, size_t& pos) {
    BlockHeader h;
    h.version = read_uint32(in, pos);
    if (pos + 32 > in.size()) throw std::runtime_error("BlockHeader::deserialize overflow prevBlock");
    for (size_t i=0;i<32;i++) h.prevBlock.data[i] = in[pos+i];
    pos += 32;
    if (pos + 32 > in.size()) throw std::runtime_error("BlockHeader::deserialize overflow merkle");
    for (size_t i=0;i<32;i++) h.merkleRoot.data[i] = in[pos+i];
    pos += 32;
    h.time = read_uint32(in, pos);
    h.bits = read_uint32(in, pos);
    h.nonce = read_uint32(in, pos);
    return h;
}

std::vector<uint8_t> Block::serialize() const {
    auto hdr = header.serialize();
    std::vector<uint8_t> out = hdr;
    write_varint(out, vtx.size());
    for (const auto& tx : vtx) {
        auto s = tx.serialize();
        write_varint(out, s.size());
        out.insert(out.end(), s.begin(), s.end());
    }
    return out;
}

Block Block::deserialize(const std::vector<uint8_t>& in, size_t& pos) {
    Block b;
    b.header = BlockHeader::deserialize(in, pos);
    uint64_t txcount = read_varint(in, pos);
    for (uint64_t i=0;i<txcount;i++) {
        uint64_t txlen = read_varint(in, pos);
        if (pos + txlen > in.size()) throw std::runtime_error("Block::deserialize tx overflow");
        std::vector<uint8_t> txb(in.begin()+pos, in.begin()+pos+txlen);
        size_t tpos = 0;
        Tx tx = Tx::deserialize(txb, tpos);
        b.vtx.push_back(std::move(tx));
        pos += txlen;
    }
    return b;
}

uint256 Block::headerHash() const {
    auto s = header.serialize();
    auto h = sha256d(s);
    return uint256::fromBytes(h);
}
