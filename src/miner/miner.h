#pragma once
#include "../consensus/block.h"
#include "../db/blockstore.h"

int MineBlock(Block& block, BlockStore& store, int maxNonceTries = 10000000);
