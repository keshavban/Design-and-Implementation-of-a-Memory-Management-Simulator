#include <iostream>
#include "allocator.h"

using namespace std;

int main() {
    cout << "Memory Management Simulator\n";

    init_memory(1024);

    allocator_type = AllocatorType::FIRST_FIT;

    malloc_block(200);
    malloc_block(300);
    malloc_block(100);

    dump_memory();

    free_block(2);

    dump_memory();

    stats();

    return 0;
}
