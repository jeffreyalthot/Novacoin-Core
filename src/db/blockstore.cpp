#include "blockstore.h"
#include <filesystem>
#include <fstream>

BlockStore::BlockStore(const std::string& datadir) : dir(datadir) {
    std::filesystem::create_directories(dir + "/blocks");
}

bool BlockStore::WriteBlock(const Block& b, const uint256& hash) {
    std::string path = dir + "/blocks/" + hash.toHex() + ".blk";
    std::ofstream f(path, std::ios::binary);
    if (!f.is_open()) return false;
    auto s = b.serialize();
    f.write(reinterpret_cast<const char*>(s.data()), s.size());
    return true;
}

bool BlockStore::HasBlock(const uint256& hash) const {
    std::string path = dir + "/blocks/" + hash.toHex() + ".blk";
    return std::filesystem::exists(path);
}
