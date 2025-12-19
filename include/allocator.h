#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <list>

// Represents one memory block
struct Block {
    int start;
    int size;
    bool free;
    int id;
};

// Allocation strategy
enum class AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// Global state (declared, not defined)
extern std::list<Block> memory_blocks;
extern AllocatorType allocator_type;

extern int total_memory;
extern int next_id;
extern int total_requests;
extern int successful_requests;

// Function declarations
void init_memory(int size);
void malloc_block(int size);
void free_block(int id);
void dump_memory();
void stats();

#endif
