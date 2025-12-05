#include "utils.h"
#include "block.h"
#include "transaction.h"
#include "blockchain.h"
#include "wallet.h"
#include "miner.h"
#include "bus.h"
#include "cli.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

// ================================
// Fonction principale novacoind
// ================================
int main() {
    // Initialisation du bus interne
    Bus bus;
    bus.subscribe([](const std::string& msg) { std::cout << msg << std::endl; });

    log("MAIN", "Starting Novacoin Core...");

    // Initialisation de la blockchain
    Blockchain blockchain(4, 50.0);

    // Création des wallets
    std::vector<Wallet*> wallets;
    Wallet* alice = new Wallet("Alice", bus);
    Wallet* bob = new Wallet("Bob", bus);
    wallets.push_back(alice);
    wallets.push_back(bob);

    // Initialisation du miner
    Miner miner(blockchain, bus, alice->getAddress());

    // Création d'une transaction initiale de récompense pour Bob
    Transaction genesisTx("system", bob->getAddress(), 50.0);
    genesisTx.signTransaction("system");
    bob->receiveTransaction(genesisTx);

    // Lancer le mining en background
    std::thread miningThread([&]() {
        while (true) {
            miner.minePendingTransactions();
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Pause entre blocks
        }
    });
    miningThread.detach();

    // Lancer la CLI interactive
    CLI cli(bus, blockchain, wallets);
    cli.run();

    // Cleanup wallets
    for (auto* w : wallets) delete w;

    log("MAIN", "Novacoin Core stopped.");

    return 0;
}
