// src/main.cpp
#include "BuzzDB.hpp"
#include "VectorDatabase.hpp"
#include "ShardedVectorDatabase.hpp"
#include "ApiKeyManager.hpp"
#include "VectorDBException.hpp"
#include "Vector.hpp"
#include "json.hpp"
#include "spdlog/spdlog.h"
#include "httplib.h"
#include <thread>
#include <iostream>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <future>

using json = nlohmann::json;

// Assuming BuzzDB.hpp is defined as follows
// include/BuzzDB.hpp
#ifndef BUZZDB_HPP
#define BUZZDB_HPP

#include "BufferManager.hpp"
#include "ShardedVectorDatabase.hpp"
#include "ApiKeyManager.hpp"
#include "VectorDBException.hpp"
#include <memory>

class BuzzDB {
public:
    BufferManager buffer_manager;
    std::unique_ptr<ShardedVectorDatabase> vector_db;
    ApiKeyManager api_key_manager;

    BuzzDB(size_t num_shards, size_t dimension) 
        : vector_db(std::make_unique<ShardedVectorDatabase>(num_shards, dimension)) {}
    
    // Insert a vector with a given ID
    void insert_vector(int id, const std::vector<float>& vector) {
        Vector vec(vector);
        vector_db->insert(id, vec);
    }

    // Query for nearest neighbors
    std::vector<std::pair<int, float>> query_vector(const std::vector<float>& vector, int k) {
        Vector target(vector);
        return vector_db->query(target, k);
    }
};

#endif // BUZZDB_HPP

int main() {
    // Initialize BuzzDB with desired number of shards and vector dimension
    size_t num_shards = 4;          // Adjust based on your requirements
    size_t vector_dimension = 128;  // Common for BERT embeddings
    BuzzDB db(num_shards, vector_dimension);

    // Initialize the HTTP server
    httplib::Server svr;

    // Insert Vector Endpoint
    svr.Post("/insert_vector", [&](const httplib::Request& req, httplib::Response& res) {
        // API key validation
        auto api_key_it = req.headers.find("X-API-Key");
        if (api_key_it == req.headers.end() || !db.api_key_manager.isValidApiKey(api_key_it->second)) {
            res.status = 401; // Unauthorized
            res.set_content("Unauthorized", "text/plain");
            return;
        }

        try {
            auto body = json::parse(req.body);
            int id = body["id"];
            std::vector<float> vector = body["vector"].get<std::vector<float>>();

            if (vector.size() != vector_dimension) {
                throw VectorDBException("Vector dimension mismatch", 400);
            }

            db.insert_vector(id, vector);

            spdlog::info("Inserted vector with ID {}", id);
            res.set_content("Vector Inserted Successfully", "text/plain");
        } catch (const VectorDBException& ex) {
            spdlog::error("Insert Vector Error: {}", ex.what());
            res.status = ex.getStatusCode();
            json error_response = {{"error", ex.what()}};
            res.set_content(error_response.dump(), "application/json");
        } catch (const std::exception& ex) {
            spdlog::error("Insert Vector Error: {}", ex.what());
            res.status = 500;
            json error_response = {{"error", "Internal Server Error"}};
            res.set_content(error_response.dump(), "application/json");
        }
    });

    // Query Vector Endpoint
    svr.Post("/query_vector", [&](const httplib::Request& req, httplib::Response& res) {
        // API key validation
        auto api_key_it = req.headers.find("X-API-Key");
        if (api_key_it == req.headers.end() || !db.api_key_manager.isValidApiKey(api_key_it->second)) {
            res.status = 401; // Unauthorized
            res.set_content("Unauthorized", "text/plain");
            return;
        }

        try {
            auto body = json::parse(req.body);
            std::vector<float> vector = body["vector"].get<std::vector<float>>();
            int k = body.value("k", 1);

            if (vector.size() != vector_dimension) {
                throw VectorDBException("Vector dimension mismatch", 400);
            }

            auto results = db.query_vector(vector, k);

            json response;
            for (const auto& [id, distance] : results) {
                response.push_back({{"id", id}, {"distance", distance}});
            }

            spdlog::info("Processed query for k = {}", k);
            res.set_content(response.dump(), "application/json");
        } catch (const VectorDBException& ex) {
            spdlog::error("Query Vector Error: {}", ex.what());
            res.status = ex.getStatusCode();
            json error_response = {{"error", ex.what()}};
            res.set_content(error_response.dump(), "application/json");
        } catch (const std::exception& ex) {
            spdlog::error("Query Vector Error: {}", ex.what());
            res.status = 500;
            json error_response = {{"error", "Internal Server Error"}};
            res.set_content(error_response.dump(), "application/json");
        }
    });

    // Additional endpoints can be added here (e.g., metrics, health checks)

    // Start the server
    std::cout << "BuzzDB Vector Database Server is running on http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}
