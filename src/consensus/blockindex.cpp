#include "blockindex.h"
#include "serialize.h"
#include <stdexcept>

std::vector<uint8_t> BlockIndexEntry::Serialize() const {
    std::vector<uint8_t> out;
    out.insert(out.end(), hash.data.begin(), hash.data.end());

    auto h = header.serialize();
    write_varint(out, h.size());
    out.insert(out.end(), h.begin(), h.end());

    write_uint32(out, height);
    return out;
}

BlockIndexEntry BlockIndexEntry::Deserialize(const std::vector<uint8_t>& in) {
    BlockIndexEntry out;
    size_t pos = 0;
    if (in.size() < 32) throw std::runtime_error("BlockIndexEntry: missing hash");

    for (size_t i = 0; i < 32; ++i) out.hash.data[i] = in[pos++];

    uint64_t headerLen = read_varint(in, pos);
    if (pos + headerLen > in.size()) throw std::runtime_error("BlockIndexEntry: invalid header length");

    std::vector<uint8_t> headerBytes(in.begin() + static_cast<long>(pos), in.begin() + static_cast<long>(pos + headerLen));
    size_t hpos = 0;
    out.header = BlockHeader::deserialize(headerBytes, hpos);
    pos += headerLen;

    out.height = read_uint32(in, pos);
    return out;
}
