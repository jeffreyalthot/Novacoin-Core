#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/sha.h>
#include <vector>
#include <random>
#include <iostream>

// ================================
// SHA-256 Hash
// ================================
inline std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// ================================
// Génération d'un identifiant unique aléatoire
// ================================
inline std::string generateRandomID(size_t length = 32) {
    static const char chars[] =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const size_t charsLen = sizeof(chars) - 1; // exclure le '\0'
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charsLen - 1);
    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; i++) result += chars[dis(gen)];
    return result;
}

// ================================
// Timestamp courant en secondes
// ================================
inline long currentTime() {
    return static_cast<long>(std::time(nullptr));
}

// ================================
// Conversion d'un vecteur de bytes en string hexadécimale
// ================================
inline std::string bytesToHex(const std::vector<unsigned char>& bytes) {
    std::stringstream ss;
    for (auto b : bytes) ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
    return ss.str();
}

// ================================
// Petit logger console
// ================================
inline void log(const std::string& tag, const std::string& message) {
    std::cout << "[" << tag << "] " << message << std::endl;
}