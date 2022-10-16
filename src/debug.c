#include <stdio.h>

#include "main.h"
#include "debug.h"

void
print_bin (uint value)
{
    for (uint i = 1 << 31; i > 1 << 27; i = i >> 1) 
        (value & i) ? printf("1") : printf("0");
    printf("|");
    for (uint i = 1 << 27; i > 1 << 8; i = i >> 1) 
        (value & i) ? printf("1") : printf("0");
    printf("|");
    for (uint i = 1 << 8; i > 1 << 5; i = i >> 1) 
        (value & i) ? printf("1") : printf("0");
    printf("|");
    for (uint i = 1 << 5; i > 1 << 2; i = i >> 1) 
        (value & i) ? printf("1") : printf("0");
    printf("|");
    for (uint i = 1 << 2; i > 0; i = i >> 1) 
        (value & i) ? printf("1") : printf("0");
}

void
print_bin_memory (uint * memory, uint length)
{
    for (uint i = 0; i < length; i += 1) {
        print_bin (memory[i]);
        printf ("\n");
    }
}

void
print_registers (uint * memory, uint length, uint pc)
{
#if DEBUG
    printf ("[%d]\n", pc);
    for (uint i = 0; i < length; i += 1) {
        printf("[%d]=%u\n", i, memory[i]);
    }
#endif
}

void
debug_msg (char *msg)
{
#if DEBUG
    printf ("%s", msg);
#endif
}

