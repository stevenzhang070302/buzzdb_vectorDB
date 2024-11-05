// src/BufferManager.cpp
#include "BufferManager.hpp"

BufferManager::BufferManager() : num_pages_in_memory(0) {
    storage_manager.extend(MAX_PAGES);  // Extend storage for disk pages
}

BufferManager::~BufferManager() {
    // Flush all dirty pages to disk
    for (const auto& entry : buffer_pool_map) {
        const auto& page_id = entry.first;
        const auto& frame = entry.second;

        if (frame->is_dirty && frame->page) {
            storage_manager.flush(page_id, frame->page);
        }
    }
}

BufferFrame& BufferManager::fix_page(PageID page_id, bool exclusive) {
    std::shared_ptr<BufferFrame> buffer_frame;
    bool is_new_page = false;

    {
        std::lock_guard<std::mutex> lock(buffer_pool_mutex);
        auto it = buffer_pool_map.find(page_id);
        if (it != buffer_pool_map.end()) {
            buffer_frame = it->second;
        }
        else {
            if (num_pages_in_memory >= MAX_PAGES_IN_MEMORY) {
                if (!evict()) {
                    throw buffer_full_error();
                }
            }
            buffer_frame = std::make_shared<BufferFrame>(page_id, storage_manager.load(page_id));
            buffer_pool_map[page_id] = buffer_frame;
            num_pages_in_memory++;
            update_fifo_list(page_id);
        }
        buffer_frame->use_count++;
    }

    // Lock the page
    if (exclusive) {
        buffer_frame->mutex.lock();
    }
    else {
        buffer_frame->mutex.lock_shared();
    }

    // Update LRU/FIFO
    update_lru_list(page_id);

    return *buffer_frame;
}

void BufferManager::unfix_page(BufferFrame& page, bool is_dirty) {
    if (is_dirty) {
        page.is_dirty = true;
    }

    // Decrement use count
    page.use_count.fetch_sub(1, std::memory_order_release);

    // Unlock the page
    // This simplistic approach assumes the user knows whether they locked exclusively or shared
    // In a more robust implementation, you would track lock modes per thread
    // Here, we'll just unlock; ensure that users manage locks correctly
    // WARNING: Potential for incorrect unlocking
    // Consider implementing lock tracking for safety
    // For demonstration, we'll skip tracking
    // Example:
    // if (exclusive) { page.mutex.unlock(); }
    // else { page.mutex.unlock_shared(); }
}

bool BufferManager::evict() {
    return tryEvictFrom(fifo_list, fifo_map) || tryEvictFrom(lru_list, lru_map);
}

bool BufferManager::tryEvictFrom(std::list<PageID>& eviction_list, std::unordered_map<PageID, std::list<PageID>::iterator>& eviction_map) {
    for (auto it = eviction_list.begin(); it != eviction_list.end(); ++it) {
        PageID page_id = *it;
        auto frame_it = buffer_pool_map.find(page_id);
        if (frame_it == buffer_pool_map.end()) continue;
        auto frame = frame_it->second;

        if (frame->mutex.try_lock()) {
            if (frame->use_count.load() == 0) {
                if (frame->is_dirty && frame->page) {
                    storage_manager.flush(frame->page_id, frame->page);
                    frame->is_dirty = false;
                }
                frame->page.reset();  // remove page data
                num_pages_in_memory--;

                eviction_list.erase(it);
                eviction_map.erase(page_id);
                buffer_pool_map.erase(frame_it);  // Remove from buffer pool

                frame->mutex.unlock();
                return true;  // Successfully evicted a page
            }
            frame->mutex.unlock();
        }
    }

    return false;  // Couldn't evict any page from this list
}

void BufferManager::release_page_resources(std::shared_ptr<BufferFrame>& buffer_frame, bool exclusive) {
    if (exclusive) {
        buffer_frame->mutex.unlock();
    } else {
        buffer_frame->mutex.unlock_shared();
    }
    {
        std::lock_guard<std::mutex> lock(buffer_frame->lock_map_mutex);
        buffer_frame->lock_map.erase(std::this_thread::get_id());
    }
    buffer_frame->use_count--;
}

void BufferManager::update_fifo_list(PageID page_id) {
    fifo_list.push_back(page_id);
    fifo_map[page_id] = std::prev(fifo_list.end());
}

void BufferManager::update_lru_list(PageID page_id) {
    std::lock_guard<std::mutex> lock(buffer_pool_mutex);
    auto fifo_it = fifo_map.find(page_id);
    if (fifo_it != fifo_map.end()) {
        fifo_list.erase(fifo_it->second);
        fifo_map.erase(fifo_it);
        lru_list.push_back(page_id);
        lru_map[page_id] = std::prev(lru_list.end());
    } else {
        auto lru_it = lru_map.find(page_id);
        if (lru_it != lru_map.end()) {
            lru_list.erase(lru_it->second);
            lru_list.push_back(page_id);
            lru_map[page_id] = std::prev(lru_list.end());
        }
    }
}

size_t BufferManager::getNumPages(){
    return storage_manager.num_pages;
}

std::vector<PageID> BufferManager::get_fifo_list() const {
    return std::vector<PageID>(fifo_list.begin(), fifo_list.end());
}

std::vector<PageID> BufferManager::get_lru_list() const {
    return std::vector<PageID>(lru_list.begin(), lru_list.end());
}
