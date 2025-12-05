#include "validation.h"
#include "utxo.h"
#include "merkle.h"
#include <iostream>

bool ValidateTransactionBasic(const Tx& tx, const UTXOSet& utxo) {
    uint64_t in_sum = 0;
    for (const auto& in : tx.vin) {
        auto eopt = utxo.Get(in.prev.txid, in.prev.index);
        if (!eopt) {
            std::cerr << "Tx validation failed: referenced UTXO not found " << in.prev.txid << ":" << in.prev.index << "
";
            return false;
        }
        in_sum += eopt->value;
    }
    uint64_t out_sum = 0;
    for (const auto& o : tx.vout) out_sum += o.value;
    if (out_sum > in_sum) {
        std::cerr << "Tx validation failed: outputs exceed inputs" << std::endl;
        return false;
    }
    return true;
}

bool ApplyTransactionToUTXO(const Tx& tx, UTXOSet& utxo) {
    for (const auto& in : tx.vin) {
        if (!utxo.Spend(in.prev.txid, in.prev.index)) {
            std::cerr << "ApplyTransactionToUTXO: spend failed" << std::endl;
            return false;
        }
    }
    auto txid = tx.txid();
    for (uint32_t i = 0; i < tx.vout.size(); ++i) {
        UTXOEntry e{tx.vout[i].value, tx.vout[i].scriptPubKey};
        utxo.Add(txid, i, e);
    }
    return true;
}

bool ValidateBlockBasic(const Block& block, const UTXOSet& utxo, bool apply) {
    auto calc = merkleRootFromTxs(block.vtx);
    if (calc != block.header.merkleRoot) {
        std::cerr << "Block validation failed: merkle root mismatch" << std::endl;
        return false;
    }
    UTXOSet temp = utxo;
    for (const auto& tx : block.vtx) {
        if (!ValidateTransactionBasic(tx, temp)) return false;
        if (!ApplyTransactionToUTXO(tx, temp)) return false;
    }
    return true;
}
