// include/ApiKeyManager.hpp
#ifndef APIKEYMANAGER_HPP
#define APIKEYMANAGER_HPP

#include <unordered_set>
#include <string>
#include <mutex>

class ApiKeyManager {
private:
    std::unordered_set<std::string> valid_api_keys;
    std::mutex api_mutex;

public:
    ApiKeyManager();
    bool isValidApiKey(const std::string& key);
    void addApiKey(const std::string& key);
    void removeApiKey(const std::string& key);
};

#endif // APIKEYMANAGER_HPP
