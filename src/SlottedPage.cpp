// src/SlottedPage.cpp
#include "SlottedPage.hpp"

// Constructor
SlottedPage::SlottedPage() : page_data(std::make_unique<char[]>(PAGE_SIZE)), metadata_size(sizeof(Slot) * MAX_SLOTS) {
    Slot* slot_array = reinterpret_cast<Slot*>(page_data.get());
    for (size_t slot_itr = 0; slot_itr < MAX_SLOTS; slot_itr++) {
        slot_array[slot_itr].empty = true;
        slot_array[slot_itr].offset = INVALID_VALUE;
        slot_array[slot_itr].length = INVALID_VALUE;
    }
}

// Add Tuple
bool SlottedPage::addTuple(std::unique_ptr<Tuple> tuple) {
    auto serializedTuple = tuple->serialize();
    size_t tuple_size = serializedTuple.size();

    size_t slot_itr = 0;
    Slot* slot_array = reinterpret_cast<Slot*>(page_data.get());        
    for (; slot_itr < MAX_SLOTS; slot_itr++) {
        if (slot_array[slot_itr].empty == true) {
            break;
        }
    }
    if (slot_itr == MAX_SLOTS){
        return false; // No empty slots
    }

    size_t offset = metadata_size;
    for (size_t i = 0; i < slot_itr; ++i) {
        if (!slot_array[i].empty && slot_array[i].offset + slot_array[i].length > offset) {
            offset = slot_array[i].offset + slot_array[i].length;
        }
    }

    if(offset + tuple_size > PAGE_SIZE){
        return false; // Not enough space
    }

    slot_array[slot_itr].empty = false;
    slot_array[slot_itr].offset = offset;
    slot_array[slot_itr].length = tuple_size;

    std::memcpy(page_data.get() + offset, serializedTuple.c_str(), tuple_size);

    return true;
}

// Delete Tuple
void SlottedPage::deleteTuple(size_t index) {
    Slot* slot_array = reinterpret_cast<Slot*>(page_data.get());
    if(index < MAX_SLOTS && !slot_array[index].empty){
        slot_array[index].empty = true;
        slot_array[index].offset = INVALID_VALUE;
        slot_array[index].length = INVALID_VALUE;
    }
}

// Print
void SlottedPage::print() const{
    Slot* slot_array = reinterpret_cast<Slot*>(page_data.get());
    for (size_t slot_itr = 0; slot_itr < MAX_SLOTS; slot_itr++) {
        if (!slot_array[slot_itr].empty){
            assert(slot_array[slot_itr].offset != INVALID_VALUE);
            const char* tuple_data = page_data.get() + slot_array[slot_itr].offset;
            std::istringstream iss(tuple_data);
            auto loadedTuple = Tuple::deserialize(iss);
            std::cout << "Slot " << slot_itr << " : [Offset: " << slot_array[slot_itr].offset 
                      << ", Length: " << slot_array[slot_itr].length << "] :: ";
            loadedTuple->print();
        }
    }
    std::cout << "\n";
}
