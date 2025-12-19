#include "allocator.h"
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

// Global variable definitions
list<Block> memory_blocks;
AllocatorType allocator_type = AllocatorType::FIRST_FIT;

int total_memory = 0;
int next_id = 1;
int total_requests = 0;
int successful_requests = 0;

// Helper function (internal use only)
static list<Block>::iterator find_block(int size) {
    auto result = memory_blocks.end();

    if (allocator_type == AllocatorType::FIRST_FIT) {
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); it++) {
            if (it->free && it->size >= size)
                return it;
        }
    }
    else if (allocator_type == AllocatorType::BEST_FIT) {
        int best_size = INT_MAX;
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); it++) {
            if (it->free && it->size >= size && it->size < best_size) {
                best_size = it->size;
                result = it;
            }
        }
    }
    else { // WORST_FIT
        int worst_size = -1;
        for (auto it = memory_blocks.begin(); it != memory_blocks.end(); it++) {
            if (it->free && it->size >= size && it->size > worst_size) {
                worst_size = it->size;
                result = it;
            }
        }
    }
    return result;
}

void init_memory(int size) {
    memory_blocks.clear();
    memory_blocks.push_back({0, size, true, -1});
    total_memory = size;
    next_id = 1;
    total_requests = 0;
    successful_requests = 0;
}

void malloc_block(int size) {
    total_requests++;

    auto it = find_block(size);
    if (it == memory_blocks.end()) {
        cout << "Allocation failed\n";
        return;
    }

    successful_requests++;

    int alloc_start = it->start;

    if (it->size == size) {
        it->free = false;
        it->id = next_id;
    } else {
        Block used = {it->start, size, false, next_id};
        Block free = {it->start + size, it->size - size, true, -1};

        it = memory_blocks.erase(it);
        memory_blocks.insert(it, free);
        memory_blocks.insert(it, used);
    }

    cout << "Allocated block id=" << next_id++
         << " at address=0x" << hex << alloc_start << dec << "\n";
}

void free_block(int id) {
    for (auto it = memory_blocks.begin(); it != memory_blocks.end(); it++) {
        if (!it->free && it->id == id) {
            it->free = true;
            it->id = -1;

            // Merge with next
            auto next_it = next(it);
            if (next_it != memory_blocks.end() && next_it->free) {
                it->size += next_it->size;
                memory_blocks.erase(next_it);
            }

            // Merge with previous
            if (it != memory_blocks.begin()) {
                auto prev_it = prev(it);
                if (prev_it->free) {
                    prev_it->size += it->size;
                    memory_blocks.erase(it);
                }
            }

            cout << "Block " << id << " freed and merged\n";
            return;
        }
    }
    cout << "Invalid block id\n";
}

void dump_memory() {
    for (auto &b : memory_blocks) {
        cout << "[0x" << hex << b.start << " - 0x"
             << (b.start + b.size - 1) << "] ";
        if (b.free)
            cout << "FREE\n";
        else
            cout << "USED (id=" << dec << b.id << ")\n";
    }
}

 void stats() {
    int used = 0, free = 0, largest_free = 0;

    for (auto &b : memory_blocks) {
        if (b.free) {
            free += b.size;
            largest_free = max(largest_free, b.size);
        } else {
            used += b.size;
        }
    }

    double external_frag = (free == 0) ? 0 :
        1.0 - (double)largest_free / free;

    cout << "Total memory: " << total_memory << "\n";
    cout << "Used memory: " << used << "\n";
    cout << "Memory utilization: "
         << (double)used / total_memory * 100 << "%\n";
    cout << "External fragmentation: "
         << external_frag * 100 << "%\n";
    cout << "Allocation success rate: "
         << (double)successful_requests / total_requests * 100 << "%\n";
}

