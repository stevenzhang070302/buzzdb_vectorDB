// include/BufferManager.hpp
#ifndef BUFFERMANAGER_HPP
#define BUFFERMANAGER_HPP

#include "BufferFrame.hpp"
#include "StorageManager.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <iostream>

static constexpr size_t MAX_PAGES_IN_MEMORY = 1000; // Define maximum pages in memory

class buffer_full_error : public std::exception {
public:
    const char* what() const noexcept override { return "Buffer is full"; }
};

class BufferManager {
private:
    StorageManager storage_manager;
    std::unordered_map<PageID, std::shared_ptr<BufferFrame>> buffer_pool_map; // Map from page id to buffer frame
    std::mutex buffer_pool_mutex; // Mutex to manage buffer pool access

    // FIFO and LRU queues and their maps
    std::list<PageID> fifo_list;
    std::unordered_map<PageID, std::list<PageID>::iterator> fifo_map;
    std::list<PageID> lru_list;
    std::unordered_map<PageID, std::list<PageID>::iterator> lru_map;

    size_t num_pages_in_memory;

    bool evict();
    bool tryEvictFrom(std::list<PageID>& eviction_list, std::unordered_map<PageID, std::list<PageID>::iterator>& eviction_map);
    void release_page_resources(std::shared_ptr<BufferFrame>& buffer_frame, bool exclusive);
    void update_fifo_list(PageID page_id);
    void update_lru_list(PageID page_id);

public:
    BufferManager();
    ~BufferManager();

    BufferFrame& fix_page(PageID page_id, bool exclusive);
    void unfix_page(BufferFrame& page, bool is_dirty);
    void extend();
    size_t getNumPages();

    std::vector<PageID> get_fifo_list() const;
    std::vector<PageID> get_lru_list() const;
};

#endif // BUFFERMANAGER_HPP
