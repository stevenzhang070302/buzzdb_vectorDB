// src/StorageManager.cpp
#include "StorageManager.hpp"

StorageManager::StorageManager() : num_pages(0) {
    fileStream.open(database_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!fileStream) {
        // If file does not exist, create it
        fileStream.clear(); // Reset the state
        fileStream.open(database_filename, std::ios::out | std::ios::binary);
        fileStream.close();
        fileStream.open(database_filename, std::ios::in | std::ios::out | std::ios::binary); 
    }

    fileStream.seekg(0, std::ios::end);
    num_pages = fileStream.tellg() / PAGE_SIZE;

    if(num_pages == 0){
        extend();
    }
}

StorageManager::~StorageManager() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

std::unique_ptr<SlottedPage> StorageManager::load(uint16_t page_id) {
    std::lock_guard<std::mutex> io_guard(io_mutex);
    assert(page_id < num_pages);
    
    fileStream.seekg(page_id * PAGE_SIZE, std::ios::beg);
    auto page = std::make_unique<SlottedPage>();
    if(fileStream.read(page->page_data.get(), PAGE_SIZE)){
        // Successfully read
    }
    else{
        std::cerr << "Error: Unable to read data from the file :: page id "<<page_id<<" \n";
        exit(-1);
    }
    return page;
}

void StorageManager::flush(uint16_t page_id, const std::unique_ptr<SlottedPage>& page) {
    std::lock_guard<std::mutex> io_guard(io_mutex);
    size_t page_offset = page_id * PAGE_SIZE;        

    fileStream.seekp(page_offset, std::ios::beg);
    fileStream.write(page->page_data.get(), PAGE_SIZE);        
    fileStream.flush();
}

void StorageManager::extend() {
    std::lock_guard<std::mutex> io_guard(io_mutex);
    std::cout << "Extending database file \n";

    auto empty_slotted_page = std::make_unique<SlottedPage>();

    fileStream.seekp(0, std::ios::end);
    fileStream.write(empty_slotted_page->page_data.get(), PAGE_SIZE);
    fileStream.flush();

    num_pages += 1;
}

void StorageManager::extend(uint64_t till_page_id) {
    std::lock_guard<std::mutex> io_guard(io_mutex); 
    uint64_t write_size = std::max(static_cast<uint64_t>(0), till_page_id + 1 - num_pages) * PAGE_SIZE;
    if(write_size > 0 ) {
        std::cout << "Extending database file till page id : "<<till_page_id<<" \n";
        char* buffer = new char[write_size];
        std::memset(buffer, 0, write_size);

        fileStream.seekp(0, std::ios::end);
        fileStream.write(buffer, write_size);
        fileStream.flush();
        
        delete[] buffer;
        num_pages = till_page_id+1;
    }
}
