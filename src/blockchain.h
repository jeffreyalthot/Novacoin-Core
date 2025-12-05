#pragma once
#include <vector>
#include "block.h"

// ================================
// Gestion complète de la blockchain
// ================================
class Blockchain {
public:
    Blockchain(int difficulty = 4, double miningReward = 50.0);

    void addBlock(Block newBlock);                // Ajoute un bloc avec PoW
    Block getLatestBlock() const;                 // Retourne le dernier bloc
    bool isChainValid() const;                    // Validation complète
    std::vector<Block> getChain() const;          // Retourne toute la chaîne

    int getDifficulty() const;                     // Difficulté actuelle
    void setDifficulty(int newDifficulty);        // Modifier difficulté
    double getMiningReward() const;               // Récompense minière

private:
    std::vector<Block> chain;
    int difficulty;
    double miningReward;

    Block createGenesisBlock();                    // Bloc genesis
};
