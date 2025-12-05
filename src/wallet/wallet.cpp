#include "wallet.h"
#include "utils.h"
#include <iostream>

// ================================
// Constructeur
// ================================
Wallet::Wallet(const std::string& n, Bus& b) : name(n), bus(b), balance(0.0) {
    privateKey = generateRandomID(64);     // Génération aléatoire de clé privée
    address = sha256(privateKey).substr(0, 34); // Adresse publique simulée (hash de la clé)
    bus.publish("[WALLET] Wallet created for " + name + " with address: " + address);
}

// ================================
// Retourne l'adresse
// ================================
std::string Wallet::getAddress() const {
    return address;
}

// ================================
// Retourne la balance
// ================================
double Wallet::getBalance() const {
    return balance;
}

// ================================
// Crée une transaction signée
// ================================
Transaction Wallet::createTransaction(const std::string& to, double amount, const std::string& privKey) {
    if (amount > balance) {
        bus.publish("[WALLET] ERROR: Insufficient balance for transaction");
        throw std::runtime_error("Insufficient balance");
    }
    Transaction tx(address, to, amount);
    tx.signTransaction(privKey.empty() ? privateKey : privKey);
    bus.publish("[WALLET] Transaction created: " + tx.txid);
    return tx;
}

// ================================
// Recevoir transaction et mettre à jour balance
// ================================
void Wallet::receiveTransaction(const Transaction& tx) {
    if (tx.toAddress == address && tx.isValid()) {
        balance += tx.amount;
        bus.publish("[WALLET] Transaction received: " + tx.txid + ", new balance: " + std::to_string(balance));
    }
}
