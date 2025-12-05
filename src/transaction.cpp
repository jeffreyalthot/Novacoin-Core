#include "transaction.h"
#include "utils.h"
#include <sstream>
#include <iostream>

// ================================
// Constructeur vide
// ================================
Transaction::Transaction() : fromAddress(""), toAddress(""), amount(0.0), timestamp(currentTime()), txid(generateRandomID()), signature("") {}

// ================================
// Constructeur complet
// ================================
Transaction::Transaction(const std::string& from, const std::string& to, double amt)
    : fromAddress(from), toAddress(to), amount(amt), timestamp(currentTime()), signature("")
{
    txid = calculateHash();
}

// ================================
// Calcul du hash de la transaction
// ================================
std::string Transaction::calculateHash() const {
    std::stringstream ss;
    ss << fromAddress << toAddress << amount << timestamp;
    return sha256(ss.str());
}

// ================================
// Signature numérique (placeholder)
// ================================
void Transaction::signTransaction(const std::string& privateKey) {
    // Ici on peut utiliser une vraie signature ECDSA ou RSA
    // Pour l'instant, on met une signature fictive = hash(privateKey + txid)
    signature = sha256(privateKey + txid);
}

// ================================
// Vérification de la transaction (placeholder)
// ================================
bool Transaction::isValid() const {
    // Dans un vrai système, vérifier la signature avec la clé publique
    // Ici, si signature non vide -> valid
    return !signature.empty();
}

// ================================
// Conversion en string interne pour hash
// ================================
std::string Transaction::toString() const {
    std::stringstream ss;
    ss << fromAddress << toAddress << amount << timestamp << signature;
    return ss.str();
}

// ================================
// Sérialisation JSON complète
// ================================
std::string Transaction::serialize() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"txid\":\"" << txid << "\",";
    ss << "\"from\":\"" << fromAddress << "\",";
    ss << "\"to\":\"" << toAddress << "\",";
    ss << "\"amount\":" << amount << ",";
    ss << "\"timestamp\":" << timestamp << ",";
    ss << "\"signature\":\"" << signature << "\"";
    ss << "}";
    return ss.str();
}
