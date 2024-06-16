#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Allocate some memory
    size_t length = 4096; // One page
    void *addr = malloc(length);

    if (addr == NULL) {
        perror("malloc");
        return 1;
    }

    // Fill the memory with some data
    memset(addr, 0xAA, length);

    // Use madvise to mark the memory as not needed
    if (madvise(addr, length, MADV_DONTNEED) != 0) {
        perror("madvise");
        free(addr);
        return 1;
    }

    // Re-access the memory to force zero-filling
    // memset(addr, 0, length);

    // Print the memory content after re-accessing
    std::cout << "After madvise and re-accessing: " << std::endl;
    bool is_zeroed = true;
    for (size_t i = 0; i < length; ++i) {
        if (((unsigned char*)addr)[i] != 0) {
            is_zeroed = false;
            printf("i is %d w val 0x%x\n", i, ((unsigned char*)addr)[i]);
            break;
        }
    }
    
    if (is_zeroed) {
        std::cout << "Memory is zeroed out." << std::endl;
    } else {
        std::cout << "Memory is not zeroed out." << std::endl;
    }

    // Free the allocated memory
    free(addr);

    return 0;
}
