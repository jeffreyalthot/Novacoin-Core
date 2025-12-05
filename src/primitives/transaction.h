#pragma once
#include <string>
#include <vector>

// ================================
// Structure d'une transaction
// ================================
class Transaction {
public:
    std::string txid;                  // Identifiant unique de la transaction
    std::string fromAddress;           // Adresse de l'expéditeur
    std::string toAddress;             // Adresse du destinataire
    double amount;                     // Montant envoyé
    long timestamp;                    // Timestamp de création
    std::string signature;             // Signature numérique (placeholder)
    
    Transaction();
    Transaction(const std::string& from, const std::string& to, double amt);

    std::string calculateHash() const; // Calcul du hash de la transaction
    void signTransaction(const std::string& privateKey); // Placeholder pour signature
    bool isValid() const;              // Vérification signature (placeholder)
    std::string toString() const;      // String interne pour hash
    std::string serialize() const;     // Sérialisation JSON complète
};
