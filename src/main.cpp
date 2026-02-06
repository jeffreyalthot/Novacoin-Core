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

namespace {
const std::vector<std::string> kCliCommands = {
    "help",
    "createkey",
    "listkeys",
    "getkeycount",
    "getkeyexists",
    "mine",
    "show-utxos",
    "getutxocount",
    "getdatadir",
    "getblockcount",
    "gettip",
    "getdifficulty",
    "getblockindex",
    "getblockexists",
    "getbestblockhash",
    "savekeys",
    "loadkeys",
    "signmessage",
    "listcommands",
    "getcommandcount"
};
}

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
                      << "  getblockexists <hash>\n"
                      << "                 Check if a block file exists on disk\n"
                      << "  getbestblockhash\n"
                      << "                 Print tip hash without label (or none)\n"
                      << "  savekeys\n"
                      << "                 Persist keystore to <datadir>/keystore.dat\n"
                      << "  loadkeys\n"
                      << "                 Reload keystore from <datadir>/keystore.dat\n"
                      << "  signmessage <keyid> <message>\n"
                      << "                 Sign a message with a known key identifier\n"
                      << "  listcommands\n"
                      << "                 List all available novacoin-cli commands\n"
                      << "  getcommandcount\n"
                      << "                 Print the total number of available commands\n";
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
            uint256 hash;
            try {
                hash = uint256::fromHex(argv[2]);
            } catch (const std::exception&) {
                std::cerr << "Invalid block hash format (expected 64-char hex)\n";
                return 1;
            }
            auto entry = chain.GetBlockIndex(hash);
            if (!entry) {
                std::cout << "Block index entry not found\n";
                return 1;
            }
            std::cout << "Hash: " << entry->hash.toHex() << "\n"
                      << "Height: " << entry->height << "\n"
                      << "Bits: " << entry->header.bits << "\n";
            return 0;
        } else if (cmd == "getblockexists") {
            if (argc < 3) {
                std::cerr << "Usage: novacoin-cli getblockexists <hash>\n";
                return 1;
            }
            uint256 hash;
            try {
                hash = uint256::fromHex(argv[2]);
            } catch (const std::exception&) {
                std::cerr << "Invalid block hash format (expected 64-char hex)\n";
                return 1;
            }
            const bool exists = chain.GetBlockStore().HasBlock(hash);
            std::cout << "Block exists: " << (exists ? "yes" : "no") << "\n";
            return exists ? 0 : 1;
        } else if (cmd == "getbestblockhash") {
            auto tip = chain.GetTip();
            std::cout << (tip ? tip->hash.toHex() : "none") << "\n";
            return tip ? 0 : 1;
        } else if (cmd == "savekeys") {
            const std::string path = cfg.data_dir + "/keystore.dat";
            if (!keystore.SaveToFile(path)) {
                std::cerr << "Failed to save keystore: " << path << "\n";
                return 1;
            }
            std::cout << "Keystore saved: " << path << "\n";
            return 0;
        } else if (cmd == "loadkeys") {
            const std::string path = cfg.data_dir + "/keystore.dat";
            if (!keystore.LoadFromFile(path)) {
                std::cerr << "Failed to load keystore: " << path << "\n";
                return 1;
            }
            std::cout << "Keystore loaded: " << path << "\n";
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
        } else if (cmd == "listcommands") {
            for (const auto& name : kCliCommands) {
                std::cout << name << "\n";
            }
            return 0;
        } else if (cmd == "getcommandcount") {
            std::cout << "Command count: " << kCliCommands.size() << "\n";
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
