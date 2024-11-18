# BuzzDB_VectorDB

Currently Still in progress - A lot of bug fixes!!

## Overview

BuzzDB_VectorDB is a high-performance, scalable vector database implemented in C++. It is designed to support high-dimensional vector operations essential for Large Language Models (LLMs) and AI applications. Leveraging advanced data structures like KD-trees, it provides efficient storage, indexing, and querying capabilities.

## Features

- **High-Dimensional Vector Storage and Retrieval**
- **Approximate Nearest Neighbor Search using KD-tree**
- **Scalability through Data Sharding**
- **Multithreading and Concurrency Control**
- **RESTful API for Easy Integration**
- **Memory Optimization using Vector Quantization**
- **Comprehensive Logging and Monitoring**
- **Dockerized Deployment for Consistent Environments**

## Installation

### Prerequisites

- **C++17 Compiler**
- **CMake (>= 3.10)**
- **Eigen3 Library**
- **OpenSSL**
- **spdlog**
- **httplib (included via Docker)**
- **nlohmann/json (included via Docker)**
- **Docker (optional, for containerization)**

### Building from Source

1. **Clone the Repository**

   ```bash
   git clone https://github.com/stevenzhang070302/BuzzDB_VectorDB.git
   cd BuzzDB_VectorDB
