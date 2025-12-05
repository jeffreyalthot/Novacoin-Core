#include "cli.h"
#include "utils.h"
#include <iostream>
#include <sstream>

// ================================
// Constructeur CLI
// ================================
CLI::CLI(Bus& b, Blockchain& bc, std::vector<Wallet*>& w)
    : bus(b), blockchain(bc), wallets(w) {}

// ================================
// Boucle principale CLI
// ================================
void CLI::run() {
    std::string line;
    while (true) {
        std::cout << "novacoin> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        executeCommand(line);
    }
}

// ================================
// Exécuter commande CLI
// ================================
void CLI::executeCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    if (tokens.empty()) return;

    if (tokens[0] == "getblockcount") {
        cmdGetBlockCount();
    } else if (tokens[0] == "getblock" && tokens.size() == 2) {
        cmdGetBlock(tokens[1]);
    } else if (tokens[0] == "sendtoaddress" && tokens.size() == 4) {
        std::string from = tokens[1];
        std::string to = tokens[2];
        double amount = std::stod(tokens[3]);
        cmdSendToAddress(from, to, amount);
    } else if (tokens[0] == "getbalance" && tokens.size() == 2) {
        cmdGetBalance(tokens[1]);
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}

// ================================
// Commandes implémentées
// ================================
void CLI::cmdGetBlockCount() {
    std::cout << "Block count: " << blockchain.getChain().size() << std::endl;
}

void CLI::cmdGetBlock(const std::string& hash) {
    for (const auto& block : blockchain.getChain()) {
        if (block.hash == hash) {
            std::cout << block.serialize() << std::endl;
            return;
        }
    }
    std::cout << "Block not found" << std::endl;
}

void CLI::cmdSendToAddress(const std::string& fromAddr, const std::string& toAddr, double amount) {
    Wallet* sender = findWalletByAddress(fromAddr);
    Wallet* receiver = findWalletByAddress(toAddr);
    if (!sender || !receiver) {
        std::cout << "Wallet not found" << std::endl;
        return;
    }

    try {
        Transaction tx = sender->createTransaction(toAddr, amount, "");
        receiver->receiveTransaction(tx);
        bus.publish("[CLI] Transaction sent: " + tx.txid);
    } catch (const std::exception& e) {
        std::cout << "Error sending transaction: " << e.what() << std::endl;
    }
}

void CLI::cmdGetBalance(const std::string& addr) {
    Wallet* w = findWalletByAddress(addr);
    if (w) {
        std::cout << "Balance of " << addr << ": " << w->getBalance() << std::endl;
    } else {
        std::cout << "Wallet not found" << std::endl;
    }
}

// ================================
// Trouver wallet par adresse
// ================================
Wallet* CLI::findWalletByAddress(const std::string& addr) {
    for (auto* w : wallets) {
        if (w->getAddress() == addr) return w;
    }
    return nullptr;
}
