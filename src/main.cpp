#include "novacoin.h"
#include "consensus/blockchain.h"
#include "consensus/block.h"
#include "consensus/tx.h"
#include "wallet/keystore.h"
#include "consensus/merkle.h"
#include "miner/miner.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

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
                      << "  getkeycount     Print number of known key identifiers\n"
                      << "  getkeyexists <keyid>\n"
                      << "                 Check if a key identifier exists in keystore\n"
                      << "  mine            Mine one prototype block\n"
                      << "  show-utxos      Print current UTXO count\n"
                      << "  getutxocount    Alias of show-utxos\n"
                      << "  getdatadir      Print the active data directory\n"
                      << "  getblockcount   Print current chain height (block count)\n"
                      << "  gettip          Print current tip block hash\n"
                      << "  getdifficulty   Print current tip difficulty bits\n"
                      << "  getblockindex <hash>\n"
                      << "                 Print basic index details for a block hash\n"
                      << "  signmessage <keyid> <message>\n"
                      << "                 Sign a message with a known key identifier\n";
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
        } else if (cmd == "getkeycount") {
            std::cout << "Key count: " << keystore.ListKeys().size() << "\n";
            return 0;
        } else if (cmd == "getkeyexists") {
            if (argc < 3) {
                std::cerr << "Usage: novacoin-cli getkeyexists <keyid>\n";
                return 1;
            }
            const std::string target = argv[2];
            const auto keys = keystore.ListKeys();
            const bool found = std::find(keys.begin(), keys.end(), target) != keys.end();
            std::cout << "Key exists: " << (found ? "yes" : "no") << "\n";
            return found ? 0 : 1;
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
        } else if (cmd == "getutxocount") {
            std::cout << "UTXO count: " << chain.GetUTXO().Size() << "\n";
            return 0;
        } else if (cmd == "getdatadir") {
            std::cout << "Data dir: " << cfg.data_dir << "\n";
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
        } else if (cmd == "getdifficulty") {
            auto tip = chain.GetTip();
            if (!tip) {
                std::cout << "Difficulty bits: none\n";
            } else {
                std::cout << "Difficulty bits: " << tip->header.bits << "\n";
            }
            return 0;
        } else if (cmd == "getblockindex") {
            if (argc < 3) {
                std::cerr << "Usage: novacoin-cli getblockindex <hash>\n";
                return 1;
            }
            auto hash = uint256::fromHex(argv[2]);
            auto entry = chain.GetBlockIndex(hash);
            if (!entry) {
                std::cout << "Block index entry not found\n";
                return 1;
            }
            std::cout << "Hash: " << entry->hash.toHex() << "\n"
                      << "Height: " << entry->height << "\n"
                      << "Bits: " << entry->header.bits << "\n";
            return 0;
        } else if (cmd == "signmessage") {
            if (argc < 4) {
                std::cerr << "Usage: novacoin-cli signmessage <keyid> <message>\n";
                return 1;
            }
            const std::string keyid = argv[2];
            std::string message = argv[3];
            for (int i = 4; i < argc; ++i) {
                message += " ";
                message += argv[i];
            }
            auto sig = keystore.Sign(message, keyid);
            if (sig.empty()) {
                std::cerr << "Unknown key id: " << keyid << "\n";
                return 1;
            }
            std::ostringstream hex;
            hex << std::hex << std::setfill('0');
            for (uint8_t b : sig) hex << std::setw(2) << static_cast<int>(b);
            std::cout << "Signature: " << hex.str() << "\n";
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
