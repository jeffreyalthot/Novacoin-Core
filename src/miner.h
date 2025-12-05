#pragma once
#include "blockchain.h"
#include "transaction.h"
#include "bus.h"
#include <vector>
#include <string>

// ================================
// Classe Miner complète
// ================================
class Miner {
public:
    Miner(Blockchain& blockchain, Bus& bus, const std::string& minerAddress);

    void addTransactionToMempool(const Transaction& tx);  // Ajouter transaction au mempool
    void minePendingTransactions();                        // Mine toutes les transactions en attente
    void setMiningReward(double reward);                  // Modifier récompense minière
    void setDifficulty(int difficulty);                   // Modifier difficulté PoW

private:
    Blockchain& blockchain;
    Bus& bus;
    std::string minerAddress;
    std::vector<Transaction> mempool;
    double miningReward;
    int difficulty;
};
