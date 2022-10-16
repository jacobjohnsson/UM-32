#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"

#define DEBUG 0

void print_bin (uint value);

void print_bin_memory (uint * memory, uint length);

void print_registers (uint * memory, uint length, uint pc);

void debug_msg (char *msg);

#endif /* _DEBUG_H_ */
