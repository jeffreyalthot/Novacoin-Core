#pragma once
#include <string>
#include "../consensus/block.h"
#include "../crypto/uint256.h"

class BlockStore {
public:
    explicit BlockStore(const std::string& datadir);
    bool WriteBlock(const Block& b, const uint256& hash);
    bool HasBlock(const uint256& hash) const;
private:
    std::string dir;
};
