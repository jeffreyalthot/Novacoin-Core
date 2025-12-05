#pragma once
#include <functional>
#include <vector>
#include <string>
#include <mutex>

// ================================
// Bus interne complet
// ================================
class Bus {
public:
    // Ajouter un subscriber (callback) pour recevoir les messages
    void subscribe(std::function<void(const std::string&)> callback);

    // Publier un message à tous les subscribers
    void publish(const std::string& message);

    // Nombre de subscribers actuellement
    size_t subscriberCount() const;

private:
    std::vector<std::function<void(const std::string&)>> subscribers;
    mutable std::mutex busMutex;
};
