#pragma once
#include <string>
#include <vector>
#include "transaction.h"
#include "bus.h"

// ================================
// Wallet complet Novacoin
// ================================
class Wallet {
public:
    Wallet(const std::string& name, Bus& bus);

    std::string getAddress() const;                   // Retourne l'adresse publique
    double getBalance() const;                        // Retourne la balance simulée
    Transaction createTransaction(const std::string& to, double amount, const std::string& privateKey); // Crée transaction signée
    void receiveTransaction(const Transaction& tx);   // Recevoir transaction et mettre à jour balance

private:
    std::string address;
    std::string name;
    std::string privateKey;                           // Placeholder clé privée
    double balance;
    Bus& bus;
};
