# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(BuzzDB_VectorDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})

# Add third-party libraries if needed
# Example: hnswlib, but now using KDTree, so remove hnswlib if not used
# add_subdirectory(external/hnswlib)

# Add executable
add_executable(BuzzDB 
    src/main.cpp 
    src/Field.cpp 
    src/Tuple.cpp 
    src/SlottedPage.cpp 
    src/StorageManager.cpp 
    src/BufferFrame.cpp 
    src/BufferManager.cpp 
    src/ApiKeyManager.cpp 
    src/Vector.cpp 
    src/VectorDatabase.cpp 
    src/ShardedVectorDatabase.cpp 
    src/VectorPage.cpp 
    src/VectorDBException.cpp
    src/KDTree.cpp
)

# Link libraries
# target_link_libraries(BuzzDB hnswlib) # Remove if not using HNSW
find_package(OpenSSL REQUIRED)
target_link_libraries(BuzzDB OpenSSL::SSL OpenSSL::Crypto pthread)
