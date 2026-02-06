#pragma once
#include "block.h"
#include "utxo.h"

bool ValidateTransactionBasic(const Tx& tx, const UTXOSet& utxo);
bool ApplyTransactionToUTXO(const Tx& tx, UTXOSet& utxo);
bool ValidateBlockBasic(const Block& block, const UTXOSet& utxo, bool apply);
