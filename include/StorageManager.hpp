// include/StorageManager.hpp
#ifndef STORAGEMANAGER_HPP
#define STORAGEMANAGER_HPP

#include "SlottedPage.hpp"
#include <fstream>
#include <memory>
#include <mutex>
#include <iostream>

using PageID = uint64_t;

class StorageManager {
public:
    std::fstream fileStream;
    size_t num_pages;
    std::mutex io_mutex;
    const std::string database_filename = "buzzdb.dat";

public:
    StorageManager();
    ~StorageManager();
    std::unique_ptr<SlottedPage> load(uint16_t page_id);
    void flush(uint16_t page_id, const std::unique_ptr<SlottedPage>& page);
    void extend();
    void extend(uint64_t till_page_id);
};

#endif // STORAGEMANAGER_HPP
