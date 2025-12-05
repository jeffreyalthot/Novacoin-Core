#include "novacoin.h"
#include "consensus/blockchain.h"
#include "consensus/block.h"
#include "consensus/tx.h"
#include "wallet/keystore.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    Config cfg = Config::Default();
    if (std::ifstream("novacoin.conf").good()) cfg.LoadFromFile("novacoin.conf");

    std::cout << "Novacoin-Core (next iteration) - Windows\n";
    std::cout << "Data dir: " << cfg.data_dir << "\n";

    Blockchain chain(cfg.data_dir);
    KeyStore keystore;
    KeyStore_LoadFromDefault(keystore, cfg.data_dir);

    if (argc >= 2) {
        std::string cmd = argv[1];
        if (cmd == "createkey") {
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