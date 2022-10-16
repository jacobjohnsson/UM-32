#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "debug.h"

#define NBR_ARRAYS 2^8
#define MEMORY_SIZE 2^16
#define NBR_REGISTERS 8
#define OP_SHIFT 28
#define REG_A_SHIFT 6
#define REG_B_SHIFT 3
#define REG_C_SHIFT 0
#define REG_A_MASK 7 << REG_A_SHIFT
#define REG_B_MASK 7 << REG_B_SHIFT
#define REG_C_MASK 7 << REG_C_SHIFT
#define REG_A(instruction) (instruction & REG_A_MASK) >> REG_A_SHIFT;
#define REG_B(instruction) (instruction & REG_B_MASK) >> REG_B_SHIFT;
#define REG_C(instruction) (instruction & REG_C_MASK) >> REG_C_SHIFT;

#define INSTRUCTION(op_code, a, b, c) op_code << OP_SHIFT | a << REG_A_SHIFT | b << REG_B_SHIFT | c << REG_C_SHIFT

enum OP_CODES {
    CONDITIONAL_MOVE,
    ARRAY_INDEX,
    ARRAY_AMENDMENT,
    ADDITION,
    MULTIPLICATION,
    DIVISION,
    NOT_AND,
    /* The following ignore some or all of A, B or C */
    HALT,
    ALLOCATION,
    ABANDONMENT,
    OUTPUT,
    INPUT,
    LOAD_PROGRAM,
    ORTHOGRAPHY,
    SUBTRACTION,
    MODULO
};

int
parse_program (char *file_path, uint *program)
{
    char op_code_str[5] = { 0 };
    char comment[128];
    uint r_a;
    uint r_b;
    uint r_c;
    uint i;

    FILE *file = fopen (file_path, "r");
    if (file == NULL) {
        printf ("No such file");
        return FALSE;
    }

    while (fscanf (file, "%s", op_code_str) == 1) {
        if (strncmp (op_code_str, "#", 1) == 0) {
            fgets (comment, 128, file);
            continue;
        }
        char msg[20];
        sprintf (msg, "op_code: %s\n", op_code_str);
        debug_msg (msg);

        if (strcmp (op_code_str, "CONM") == 0) {
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (CONDITIONAL_MOVE, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "ARIN") == 0) {
            printf ("ARRAY INDEX not yet implemented.");
            return FALSE;
        } else if (strcmp (op_code_str, "ARAM") == 0) {
            printf ("ARRAY AMENDMENT not yet implemented.");
            return FALSE;
        } else if (strcmp (op_code_str, "ADDI") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (ADDITION, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "MULT") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (MULTIPLICATION, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "DIVI") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (DIVISION, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "NAND") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (NOT_AND, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "HALT") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (HALT, 0, 0, 0);
        } else if (strcmp (op_code_str, "ALLO") == 0){
            printf ("ALLOCATION not yet implemented.");
            return FALSE;
        } else if (strcmp (op_code_str, "ABAN") == 0){
            printf ("ABANDONMENT not yet implemented.");
            return FALSE;
        } else if (strcmp (op_code_str, "OUTP") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (OUTPUT, 0, 0, r_c);
        } else if (strcmp (op_code_str, "INPU") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (INPUT, 0, 0, r_c);
        } else if (strcmp (op_code_str, "LDPR") == 0){
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (LOAD_PROGRAM, 0, r_b, r_c);
        } else if (strcmp (op_code_str, "ORTH") == 0) {
            uint value;
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &value);
            program[i] = ORTHOGRAPHY << OP_SHIFT | r_a << 25 | value;
        } else if (strcmp (op_code_str, "SUBT") == 0) {
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (SUBTRACTION, r_a, r_b, r_c);
        } else if (strcmp (op_code_str, "MODU") == 0) {
            fscanf (file, "%u", &r_a);
            fscanf (file, "%u", &r_b);
            fscanf (file, "%u", &r_c);
            program[i] = INSTRUCTION (MODULO, r_a, r_b, r_c);
        } else {
            printf ("Unknown op_code %s in %s.\n", op_code_str, file_path); 
            return FALSE;
        }
        i++;
    }
}

int execute (uint *memory[])
{
    uint pc = 0;
    uint running = 1;
    uint r_a;
    uint r_b;
    uint r_c;
    uint offset;
    uint array_target;
    uint registers[NBR_REGISTERS] = { 0 };

    uint *program = memory[0];

    while (running) {
        r_a = REG_A (program[pc]);
        r_b = REG_B (program[pc]);
        r_c = REG_C (program[pc]);
        switch (program[pc] >> OP_SHIFT) {
            case CONDITIONAL_MOVE:
                debug_msg ("CONDITIONAL_MOVE\n");
                if (registers[r_c] != 0) {
                    registers[r_a] = registers[r_b];
                }
                break;
            case ARRAY_INDEX:
                debug_msg ("ARRAY_INDEX\n");
                offset = registers[r_c];
                array_target = registers[r_b];
                if (array_target == 0) {
                    return EXIT_FAILURE;
                }
                registers[r_a] = memory[array_target][offset];
                break;
            case ARRAY_AMENDMENT:
                debug_msg ("ARRAY_AMENDMENT\n");
                offset = registers[r_b];
                array_target = registers[r_a];
                if (array_target == 0) {
                    return EXIT_FAILURE;
                }
                memory[array_target][offset] = registers[r_c];
                break;
            case ADDITION:
                debug_msg ("ADDITION\n");
                registers[r_a] = registers[r_b] + registers[r_c];
                break;
            case MULTIPLICATION:
                debug_msg ("MULTIPLICATION\n");
                registers[r_a] = registers[r_b] * registers[r_c]; 
                break;
            case DIVISION:
                debug_msg ("DIVISION\n");
                registers[r_a] = registers[r_b] / registers[r_c];
                break;
            case NOT_AND:
                debug_msg ("NOT_AND\n");
                registers[r_a] = ~(registers[r_b] & registers[r_c]);
                break;
            case HALT:
                debug_msg ("HALT\n");
                running = 0;
                break;
            case ALLOCATION:
                debug_msg ("ALLOCATION\n");
                break;
            case ABANDONMENT:
                debug_msg ("ABANDONMENT\n");
                break;
            case OUTPUT:
                debug_msg ("OUTPUT\n");
                printf ("%c", (char) (registers[r_c] & ((1 << 9) - 1)));
                break;
            case INPUT:
                debug_msg ("INPUT\n");
                char c = getchar();
                getchar();
                registers[r_c] = (uint) c;
                break;
            case LOAD_PROGRAM:
                debug_msg ("LOAD_PROGRAM\n");
                program = memory[registers[r_b]];
                pc = registers[r_c] - 1;
                break;
            case ORTHOGRAPHY:
                debug_msg ("ORTHOGRAPHY\n");
                r_a = (program[pc] >> 25) & 7;
                registers[r_a] = program[pc] & ((1 << 25) - 1);
                break;
            case SUBTRACTION:
                debug_msg ("SUBTRATION\n");
                registers[r_a] = registers[r_b] - registers[r_c];
                break;
            case MODULO:
                debug_msg ("MODULO\n");
                registers[r_a] = registers[r_b] % registers[r_c];
                break;
        }
        pc++;
        print_registers (registers, 8, pc);
    }
}

int main (int argc, char * argv[]) 
{
    if (argc < 2) {
        printf ("supply file name as argument.");
        return 0;
    }

    uint *memory[NBR_ARRAYS] = { 0 };
    uint program[MEMORY_SIZE] = { 0 };

    memory[0] = program;

    if (parse_program (argv[1], program) == 0) {
        return -1;
    }

    for (int i = 0; i < 100000; i++) {
        execute (memory);
    }
}
