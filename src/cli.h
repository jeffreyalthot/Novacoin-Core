#pragma once
#include <string>
#include "bus.h"
#include "blockchain.h"
#include "wallet.h"
#include <vector>

// ================================
// CLI complet Novacoin
// ================================
class CLI {
public:
    CLI(Bus& bus, Blockchain& blockchain, std::vector<Wallet*>& wallets);

    void run();                                // Boucle principale CLI
    void executeCommand(const std::string& cmd); // Exécuter une commande spécifique

private:
    Bus& bus;
    Blockchain& blockchain;
    std::vector<Wallet*>& wallets;

    void cmdGetBlockCount();
    void cmdGetBlock(const std::string& hash);
    void cmdSendToAddress(const std::string& fromAddr, const std::string& toAddr, double amount);
    void cmdGetBalance(const std::string& addr);

    Wallet* findWalletByAddress(const std::string& addr);
};
