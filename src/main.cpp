#include "novacoin.h"
#include "consensus/blockchain.h"
#include "consensus/block.h"
#include "consensus/tx.h"
#include "wallet/keystore.h"
#include "consensus/merkle.h"
#include "miner/miner.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char** argv) {
    Config cfg = Config::Default();
    if (std::ifstream("novacoin.conf").good()) cfg.LoadFromFile("novacoin.conf");

    std::cout << "Novacoin Core - internal fork baseline\n";
    std::cout << "Data dir: " << cfg.data_dir << "\n";

    Blockchain chain(cfg.data_dir);
    KeyStore keystore;
    KeyStore_LoadFromDefault(keystore, cfg.data_dir);

    if (argc >= 2) {
        std::string cmd = argv[1];

        auto printHelp = []() {
            std::cout << "Novacoin CLI commands:\n"
                      << "  help            Show this help message\n"
                      << "  createkey       Generate a new key identifier\n"
                      << "  listkeys        List known key identifiers\n"
                      << "  mine            Mine one prototype block\n"
                      << "  show-utxos      Print current UTXO count\n"
                      << "  getblockcount   Print current chain height (block count)\n"
                      << "  gettip          Print current tip block hash\n";
        };

        if (cmd == "help" || cmd == "--help" || cmd == "-h") {
            printHelp();
            return 0;
        } else if (cmd == "createkey") {
            auto id = keystore.GenerateNew();
            std::cout << "New key id: " << id << "\n";
            return 0;
        } else if (cmd == "listkeys") {
            auto keys = keystore.ListKeys();
            for (auto &k: keys) std::cout << k << "\n";
            return 0;
        } else if (cmd == "mine") {
            Block b;
            b.header.version = 1;
            auto tip = chain.GetTip();
            if (tip) {
                b.header.prevBlock = tip->hash;
            } else {
                std::array<uint8_t,32> z{}; b.header.prevBlock = uint256::fromBytes(z);
            }
            b.header.bits = 2; // prototype difficulty
            Tx coinbase;
            TxOut out; out.value = 50 * 100000000ULL;
            std::string msg = "Novacoin genesis coinbase";
            out.scriptPubKey.assign(msg.begin(), msg.end());
            coinbase.vout.push_back(out);
            b.vtx.push_back(coinbase);
            b.header.merkleRoot = merkleRootFromTxs(b.vtx);
            // Use the blockchain's BlockStore via accessor
            int res = MineBlock(b, chain.GetBlockStore(), 2000000);
            if (res == 0) {
                if (!chain.AddBlock(b)) {
                    std::cerr << "Failed to add block to chain\n";
                    return 1;
                }
            }
            return res;
        } else if (cmd == "show-utxos") {
            std::cout << "UTXO count: " << chain.GetUTXO().Size() << "\n";
            return 0;
        } else if (cmd == "getblockcount") {
            std::cout << "Block count: " << chain.Height() << "\n";
            return 0;
        } else if (cmd == "gettip") {
            auto tip = chain.GetTip();
            if (!tip) {
                std::cout << "Tip: none\n";
            } else {
                std::cout << "Tip: " << tip->hash.toHex() << "\n";
            }
            return 0;
        } else {
            std::cerr << "Unknown command: " << cmd << "\n\n";
            printHelp();
            return 1;
        }
    }

    Novacoin node;
    if (!node.Init(cfg)) return 1;
    node.Start();
    std::cout << "Node running (mock). Press Enter to stop.\n";
    std::cin.get();
    node.Stop();
    return 0;
}
