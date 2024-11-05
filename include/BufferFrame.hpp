// include/BufferFrame.hpp
#ifndef BUFFERFRAME_HPP
#define BUFFERFRAME_HPP

#include "SlottedPage.hpp"
#include <memory>
#include <shared_mutex>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <thread>

using PageID = uint64_t;

class BufferFrame {
private:
    friend class BufferManager;

    PageID page_id; // The ID of the page
    bool is_dirty; // Flag indicating if the page has been modified
    std::atomic<int> use_count; // Number of times the page is fixed (in use)
    std::shared_mutex mutex; // To manage concurrent access
    std::mutex lock_map_mutex;
    std::unordered_map<std::thread::id, bool> lock_map; // Map from thread id to lock mode (true for exclusive, false for shared)

public:
    std::unique_ptr<SlottedPage> page;  // The actual page data in memory

    // Constructor
    BufferFrame(PageID pid, std::unique_ptr<SlottedPage> page_data = nullptr);
};

#endif // BUFFERFRAME_HPP
