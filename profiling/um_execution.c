/*
 * um_execution.c
 * Original Version by Rachel Scrivanich and Andrew Maynard
 * Update by Joel Brandinger & Sabrina Fried
 *
 * Implementation of the um_execution.h interface. Extracts all instructions,
 * keeps track of these instructions, and contains a switch statement to
 * call certain functions from um_instructions.h in accordance with a specified
 * input.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bitpack.h"
//#include "um_instructions.h"
#include "um_execution.h"

#define T Instruction_T

/* instruction declarations ================================================ */
static inline void switch_commands(T instruction, Memory_T memory,
                                   uint32_t *registers, int *prog_counter);
static inline T pack_instruction(T instruction, uint32_t word);

/* struct definition ======================================================= */
struct T {
    unsigned opcode, register_A, register_B, register_C;
};

typedef enum Um_opcode { /* way to map numbers to global variables */
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* function definition ===================================================== */

/* execute
 *
 *      Purpose: Call proper functions to execute instructions in segment 0.
 *
 *   Parameters: Instance of program memory, pointer to an array of
 *               registers, and a pointer to program counter to keep track of
 *               instructions.
 *
 *      Returns: None
 *
 * Expectations: None
*/
extern void execute(Memory_T program, uint32_t *registers, int *prog_counter)
{
    uint32_t word;

    /* allocate space for instruction struct */
    T instruction = malloc(sizeof(*instruction));

    /* runs until halt is reached or a failed case */
    while (true) {
        word = segment_load(program, 0, *prog_counter);
        instruction = pack_instruction(instruction, word);
        switch_commands(instruction, program, registers, prog_counter);
        (*prog_counter)++; /* moves to next instruction */
    }

}

/* static function definitions============================================== */

/* switch_commands
 *
 *      Purpose: Command loop to execute instruction based on given opcode.
 *
 *   Parameters: Instance of instruction struct, instance of program memory,
 *               pointer to an array of registers, and a pointer to program
 *               counter to keep track of instructions.
 *
 *      Returns: None
 *
 * Expectations: None
*/
static inline void switch_commands(T instruction, Memory_T memory,
                                  uint32_t *registers, int *prog_counter)
{
    (void)prog_counter;

    /* performs a certain instruction based on opcode */
    switch(instruction->opcode) {
        case CMOV:
            if (registers[instruction->register_C] != 0) {
                registers[instruction->register_A] = 
                            registers[instruction->register_B];
            }
            break;

        case SLOAD:
            registers[instruction->register_A] = segment_load(memory, 
                            registers[instruction->register_B], 
                            registers[instruction->register_C]);
            break;

        case SSTORE:
            segment_store(memory, registers[instruction->register_A], 
                            registers[instruction->register_B], 
                            registers[instruction->register_C]);
            break;

        case ADD:
            registers[instruction->register_A] = 
                            registers[instruction->register_B] 
                            + registers[instruction->register_C];
            break;

        case MUL:
            registers[instruction->register_A] = 
                            registers[instruction->register_B] 
                            * registers[instruction->register_C];
            break;

        case DIV:
            registers[instruction->register_A] = 
                            registers[instruction->register_B] 
                            / registers[instruction->register_C];
            break;

        case NAND:
            registers[instruction->register_A] = 
                            ~(registers[instruction->register_B] 
                            & registers[instruction->register_C]);
            break;

        case HALT:
            free(instruction);
            segment_free(memory);
            exit(EXIT_SUCCESS);
            break;

        case ACTIVATE:
            registers[instruction->register_B] = segment_map(memory, 
                            registers[instruction->register_C]);
            break;

        case INACTIVATE:
            segment_unmap(memory, registers[instruction->register_C]);
            break;

        case OUT:
            putchar(registers[instruction->register_C]);
            break;

        case IN:
            registers[instruction->register_C] = (uint32_t)getchar();
            /* checks if end of input has been reached */
            if (registers[instruction->register_C] == (uint32_t) EOF) {
                registers[instruction->register_C] = ~0;
            }
            break;

        case LOADP:
            if (registers[instruction->register_B] != 0) {
                segment_load_program(memory, 
                            registers[instruction->register_B]);
            }
            *prog_counter = registers[instruction->register_C] - 1;
            break;

        case LV:
            registers[instruction->register_A] = 
                            (uint32_t)instruction->register_C;
            break;
    }

}

/* pack_instruction
 *
 *      Purpose: Command loop to execute instruction based on given opcode.
 *
 *   Parameters: Instance of instruction struct, a word that is passed in
 *               which holds an instruction.
 *
 *      Returns: A fully extracted instruction (instruction_T).
 *
 * Expectations: Valid opcode numbered 0 through 13.
*/
static inline T pack_instruction(T instruction, uint32_t word)
{
    /* pack opcode and make sure no bigger than 14 */
    instruction->opcode = word >> 28;
    

    if (instruction->opcode == 13) {
        instruction->register_A = word << 4 >> 29;
        instruction->register_B = 0;

        /* give register C the value of the instruction */
        instruction->register_C = word << 7 >> 7;
    } else {
        instruction->register_A = word << 23 >> 29;
        instruction->register_B = word << 26 >> 29;
        instruction->register_C = word << 29 >> 29;
    }

    return instruction; /* return packed instruction */
}
