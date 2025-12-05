#include "utils.h"
#include "block.h"
#include "transaction.h"
#include "blockchain.h"
#include "wallet.h"
#include "miner.h"
#include "bus.h"
#include <vector>
#include <iostream>

int main() {
    // Initialisation du bus interne pour test
    Bus bus;
    bus.subscribe([](const std::string& msg) { std::cout << "[TEST BUS] " << msg << std::endl; });

    log("TEST", "Starting Novacoin test environment...");

    // Création blockchain test
    Blockchain blockchain(3, 50.0);

    // Création wallets test
    std::vector<Wallet*> wallets;
    Wallet* alice = new Wallet("Alice", bus);
    Wallet* bob = new Wallet("Bob", bus);
    Wallet* charlie = new Wallet("Charlie", bus);
    wallets.push_back(alice);
    wallets.push_back(bob);
    wallets.push_back(charlie);

    // Initial transactions
    Transaction t1("system", alice->getAddress(), 100.0);
    t1.signTransaction("system");
    alice->receiveTransaction(t1);

    Transaction t2("system", bob->getAddress(), 50.0);
    t2.signTransaction("system");
    bob->receiveTransaction(t2);

    // Miner pour test
    Miner miner(blockchain, bus, alice->getAddress());

    // Ajouter transactions au mempool
    Transaction tx1 = alice->createTransaction(bob->getAddress(), 20.0, "");
    miner.addTransactionToMempool(tx1);

    Transaction tx2 = bob->createTransaction(charlie->getAddress(), 10.0, "");
    miner.addTransactionToMempool(tx2);

    // Mine un bloc avec ces transactions
    miner.minePendingTransactions();

    // Ajouter d'autres transactions
    Transaction tx3 = charlie->createTransaction(alice->getAddress(), 5.0, "");
    miner.addTransactionToMempool(tx3);

    // Mine un autre bloc
    miner.minePendingTransactions();

    // Affichage de la blockchain complète
    log("TEST", "Blockchain content:");
    for (const auto& block : blockchain.getChain()) {
        std::cout << block.serialize() << std::endl;
    }

    // Validation de la blockchain
    if (blockchain.isChainValid()) {
        log("TEST", "Blockchain is valid.");
    } else {
        log("TEST", "Blockchain is INVALID!");
    }

    // Affichage balances finales
    for (auto* w : wallets) {
        std::cout << "[TEST] Wallet " << w->getAddress() << " balance: " << w->getBalance() << std::endl;
    }

    // Cleanup wallets
    for (auto* w : wallets) delete w;

    log("TEST", "Novacoin test completed.");

    return 0;
}
