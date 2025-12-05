#include "block.h"
#include "utils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>

// ================================
// Constructeur vide
// ================================
Block::Block() : index(0), previousHash("0"), hash(""), timestamp(currentTime()), nonce(0), reward(0.0), minerAddress("") {}

// ================================
// Constructeur complet
// ================================
Block::Block(int idx, const std::string& prevHash, const std::vector<Transaction>& txs, const std::string& minerAddr, double rewardVal)
    : index(idx), previousHash(prevHash), transactions(txs), minerAddress(minerAddr), reward(rewardVal), nonce(0)
{
    timestamp = currentTime();
    hash = calculateHash();
}

// ================================
// Calcul du hash du bloc
// ================================
std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index << previousHash << timestamp << nonce << minerAddress << reward;
    for (const auto& tx : transactions) ss << tx.toString();
    return sha256(ss.str());
}

// ================================
// Mining Proof-of-Work
// ================================
void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    std::cout << "[MINER] Mining block " << index << "..." << std::endl;
    long start = currentTime();
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
        // Optionnel : sleep pour simulation CPU
        if (nonce % 1000000 == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    long end = currentTime();
    std::cout << "[MINER] Block mined: " << hash << " in " << (end - start) << "s" << std::endl;
}

// ================================
// Sérialisation JSON simplifiée
// ================================
std::string Block::serialize() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"index\":" << index << ",";
    ss << "\"previousHash\":\"" << previousHash << "\",";
    ss << "\"hash\":\"" << hash << "\",";
    ss << "\"timestamp\":" << timestamp << ",";
    ss << "\"nonce\":" << nonce << ",";
    ss << "\"reward\":" << reward << ",";
    ss << "\"minerAddress\":\"" << minerAddress << "\",";
    ss << "\"transactions\":[";
    for (size_t i = 0; i < transactions.size(); i++) {
        ss << transactions[i].serialize();
        if (i < transactions.size() - 1) ss << ",";
    }
    ss << "]}";
    return ss.str();
}
