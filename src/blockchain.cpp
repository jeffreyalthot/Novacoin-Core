#include "blockchain.h"
#include "utils.h"
#include <iostream>
#include <ctime>

// ================================
// Constructeur complet
// ================================
Blockchain::Blockchain(int diff, double reward) : difficulty(diff), miningReward(reward) {
    chain.push_back(createGenesisBlock());
}

// ================================
// Création du bloc genesis
// ================================
Block Blockchain::createGenesisBlock() {
    Block genesis;
    genesis.index = 0;
    genesis.previousHash = "0";
    genesis.timestamp = currentTime();
    genesis.nonce = 0;
    genesis.reward = miningReward;
    genesis.minerAddress = "genesis";
    genesis.hash = genesis.calculateHash();
    std::cout << "[BLOCKCHAIN] Genesis block created: " << genesis.hash << std::endl;
    return genesis;
}

// ================================
// Retourne le dernier bloc
// ================================
Block Blockchain::getLatestBlock() const {
    return chain.back();
}

// ================================
// Retourne la chaîne complète
// ================================
std::vector<Block> Blockchain::getChain() const {
    return chain;
}

// ================================
// Ajout d'un bloc avec Proof-of-Work
// ================================
void Blockchain::addBlock(Block newBlock) {
    newBlock.previousHash = getLatestBlock().hash;
    std::cout << "[BLOCKCHAIN] Mining new block..." << std::endl;
    newBlock.mineBlock(difficulty);
    chain.push_back(newBlock);
    std::cout << "[BLOCKCHAIN] Block added: " << newBlock.hash << std::endl;
}

// ================================
// Validation complète de la chaîne
// ================================
bool Blockchain::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        const Block& current = chain[i];
        const Block& previous = chain[i - 1];

        if (current.hash != current.calculateHash()) {
            std::cerr << "[ERROR] Block hash mismatch at index " << i << std::endl;
            return false;
        }

        if (current.previousHash != previous.hash) {
            std::cerr << "[ERROR] Previous hash mismatch at index " << i << std::endl;
            return false;
        }

        for (const auto& tx : current.transactions) {
            if (!tx.isValid()) {
                std::cerr << "[ERROR] Invalid transaction in block " << i << std::endl;
                return false;
            }
        }
    }
    return true;
}

// ================================
// Gestion de la difficulté
// ================================
int Blockchain::getDifficulty() const { return difficulty; }
void Blockchain::setDifficulty(int newDifficulty) { difficulty = newDifficulty; }

// ================================
// Récompense minière
// ================================
double Blockchain::getMiningReward() const { return miningReward; }
