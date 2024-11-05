// src/ApiKeyManager.cpp
#include "ApiKeyManager.hpp"

ApiKeyManager::ApiKeyManager() {
    // Initialize with some default API keys (for demonstration)
    valid_api_keys.insert("your-default-api-key");
}

bool ApiKeyManager::isValidApiKey(const std::string& key) {
    std::lock_guard<std::mutex> lock(api_mutex);
    return valid_api_keys.find(key) != valid_api_keys.end();
}

void ApiKeyManager::addApiKey(const std::string& key) {
    std::lock_guard<std::mutex> lock(api_mutex);
    valid_api_keys.insert(key);
}

void ApiKeyManager::removeApiKey(const std::string& key) {
    std::lock_guard<std::mutex> lock(api_mutex);
    valid_api_keys.erase(key);
}
