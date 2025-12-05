#pragma once
#include <string>
#include <vector>
#include "transaction.h"

// ================================
// Structure d'un bloc
// ================================
class Block {
public:
    int index;                              // Numéro du bloc dans la chaîne
    std::string previousHash;               // Hash du bloc précédent
    std::string hash;                        // Hash courant du bloc
    long timestamp;                          // Timestamp de création
    int nonce;                               // Nonce pour Proof-of-Work
    std::vector<Transaction> transactions;   // Liste des transactions
    double reward;                           // Récompense du mineur
    std::string minerAddress;                // Adresse du mineur

public:
    Block();                                 // Constructeur vide
    Block(int idx, const std::string& prevHash, const std::vector<Transaction>& txs, const std::string& minerAddr, double reward);

    std::string calculateHash() const;       // Calcul du hash du bloc
    void mineBlock(int difficulty);          // Proof-of-Work
    std::string serialize() const;           // Sérialisation JSON simple
};
