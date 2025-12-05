#include "miner.h"
#include "utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

static std::mutex minerMutex;

// ================================
// Constructeur complet
// ================================
Miner::Miner(Blockchain& bc, Bus& b, const std::string& addr)
    : blockchain(bc), bus(b), minerAddress(addr)
{
    miningReward = bc.getMiningReward();
    difficulty = bc.getDifficulty();
}

// ================================
// Ajouter transaction au mempool
// ================================
void Miner::addTransactionToMempool(const Transaction& tx) {
    std::lock_guard<std::mutex> lock(minerMutex);
    if (tx.isValid()) {
        mempool.push_back(tx);
        bus.publish("[MINER] Transaction added to mempool: " + tx.txid);
    } else {
        bus.publish("[MINER] Invalid transaction rejected: " + tx.txid);
    }
}

// ================================
// Miner toutes les transactions en attente
// ================================
void Miner::minePendingTransactions() {
    std::lock_guard<std::mutex> lock(minerMutex);
    if (mempool.empty()) {
        bus.publish("[MINER] No transactions to mine. Mining reward only.");
    }

    // Préparer bloc avec toutes les transactions du mempool
    Block newBlock(blockchain.getLatestBlock().index + 1,
                   blockchain.getLatestBlock().hash,
                   mempool,
                   minerAddress,
                   miningReward);

    bus.publish("[MINER] Mining new block with " + std::to_string(mempool.size()) + " transactions...");

    // Simulation multi-thread (1 thread ici pour simplification CPU)
    std::thread miningThread([&]() {
        newBlock.mineBlock(difficulty);
        blockchain.addBlock(newBlock);
        bus.publish("[MINER] Block mined successfully: " + newBlock.hash);
    });

    miningThread.join();

    // Vider le mempool après mining
    mempool.clear();
}

// ================================
// Modifier la récompense minière
// ================================
void Miner::setMiningReward(double reward) {
    miningReward = reward;
}

// ================================
// Modifier la difficulté PoW
// ================================
void Miner::setDifficulty(int diff) {
    difficulty = diff;
}
