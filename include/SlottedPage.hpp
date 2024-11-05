// include/SlottedPage.hpp
#ifndef SLOTTEDPAGE_HPP
#define SLOTTEDPAGE_HPP

#include "Tuple.hpp"
#include <memory>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>

static constexpr size_t PAGE_SIZE = 4096;  // Fixed page size
static constexpr size_t MAX_SLOTS = 512;   // Fixed number of slots
static constexpr size_t MAX_PAGES = 1000;  // Total Number of pages that can be stored
const uint16_t INVALID_VALUE = std::numeric_limits<uint16_t>::max(); // Sentinel value

struct Slot {
    bool empty = true;                  // Is the slot empty?
    uint16_t offset = INVALID_VALUE;    // Offset of the slot within the page
    uint16_t length = INVALID_VALUE;    // Length of the slot
};

// Slotted Page class
class SlottedPage {
public:
    std::unique_ptr<char[]> page_data;
    size_t metadata_size;

public:
    SlottedPage();
    bool addTuple(std::unique_ptr<Tuple> tuple);
    void deleteTuple(size_t index);
    void print() const;
};

#endif // SLOTTEDPAGE_HPP
