// src/BufferFrame.cpp
#include "BufferFrame.hpp"

// Constructor
BufferFrame::BufferFrame(PageID pid, std::unique_ptr<SlottedPage> page_data)
    : page_id(pid), is_dirty(false), use_count(0), page(std::move(page_data)) {}
