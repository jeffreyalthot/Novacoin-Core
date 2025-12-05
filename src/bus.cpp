#include "bus.h"
#include "utils.h"
#include <iostream>

// ================================
// Ajouter un subscriber
// ================================
void Bus::subscribe(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(busMutex);
    subscribers.push_back(callback);
    log("BUS", "New subscriber added. Total: " + std::to_string(subscribers.size()));
}

// ================================
// Publier un message à tous les subscribers
// ================================
void Bus::publish(const std::string& message) {
    std::lock_guard<std::mutex> lock(busMutex);
    for (auto& cb : subscribers) {
        try {
            cb(message);
        } catch (const std::exception& e) {
            log("BUS", std::string("Error in subscriber callback: ") + e.what());
        }
    }
}

// ================================
// Nombre de subscribers
// ================================
size_t Bus::subscriberCount() const {
    std::lock_guard<std::mutex> lock(busMutex);
    return subscribers.size();
}
