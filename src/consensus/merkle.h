#pragma once
#include "../crypto/uint256.h"
#include <vector>
#include "tx.h"

uint256 merkleRootFromTxs(const std::vector<Tx>& txs);
