#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../util/error.h"

// CPU error types
typedef enum {
    CPU_FUNCTION_BODY,
    CPU_INVALID_OPCODE,
    CPU_INVALID_ADDRESS,
    CPU_HALTED,
    CPU_MEMORY_ERROR,
    CPU_REGISTER_ERROR,
} cpu_error_t;
ERROR(cpu_error_t) cpu_error;

// 6502 Status Register Flags
#define FLAG_CARRY     0x01    // Carry Flag
#define FLAG_ZERO      0x02    // Zero Flag  
#define FLAG_INTERRUPT 0x04    // Interrupt Disable Flag
#define FLAG_DECIMAL   0x08    // Decimal Mode Flag
#define FLAG_BREAK     0x10    // Break Command Flag
#define FLAG_UNUSED    0x20    // Unused Flag (always 1)
#define FLAG_OVERFLOW  0x40    // Overflow Flag
#define FLAG_NEGATIVE  0x80    // Negative Flag

// Memory size constants
#define MEMORY_SIZE    65536   // 64KB total memory
#define STACK_OFFSET   0x0100  // Stack starts at $0100
#define STACK_SIZE     256     // Stack is 256 bytes

// CPU structure
typedef struct {
    // 6502 Registers
    uint8_t ac;         // Accumulator
    uint8_t x;          // X Index Register
    uint8_t y;          // Y Index Register
    uint8_t sp;         // Stack Pointer
    uint16_t pc;        // Program Counter
    uint8_t sr;         // Status Register
    
    // Memory
    uint8_t memory[MEMORY_SIZE];
    
    // Execution state
    int cycles;         // Cycle counter
    bool halted;        // CPU halted state
} CPU_6502;

// Addressing modes
typedef enum {
    CPU_IMPLIED,            // Implied (no operand)
    CPU_IMMEDIATE,          // Immediate (#$00)
    CPU_ZERO_PAGE,          // Zero Page ($00)
    CPU_ZERO_PAGE_X,        // Zero Page X ($00,X)
    CPU_ZERO_PAGE_Y,        // Zero Page Y ($00,Y)
    CPU_ABSOLUTE,           // Absolute ($0000)
    CPU_ABSOLUTE_X,         // Absolute X ($0000,X)
    CPU_ABSOLUTE_Y,         // Absolute Y ($0000,Y)
    CPU_INDIRECT,           // Indirect (($0000))
    CPU_INDEXED_INDIRECT,   // Indexed Indirect (($00,X))
    CPU_INDIRECT_INDEXED,   // Indirect Indexed (($00),Y)
    CPU_RELATIVE            // Relative (for branches)
} CPU_AddressingMode;

// Instruction structure
typedef struct {
    uint8_t opcode;
    char* mnemonic;
    CPU_AddressingMode mode;
    int cycles;
    int bytes;
    void (*execute)(CPU_6502*, uint16_t);  // Function pointer to instruction
} CPU_Instruction;

// Function declarations
CPU_6502 cpu_init(void);
void cpu_reset(CPU_6502* cpu);
void cpu_load_program(CPU_6502* cpu, uint8_t* program, uint16_t size, uint16_t start_addr);
uint8_t cpu_fetch(CPU_6502* cpu);
uint16_t cpu_fetch_word(CPU_6502* cpu);
void cpu_execute(CPU_6502* cpu);
int cpu_step(CPU_6502* cpu);

// Memory access functions
uint8_t cpu_read(CPU_6502* cpu, uint16_t address);
void cpu_write(CPU_6502* cpu, uint16_t address, uint8_t value);
uint16_t cpu_read_word(CPU_6502* cpu, uint16_t address);
void cpu_write_word(CPU_6502* cpu, uint16_t address, uint16_t value);

// Stack operations
void cpu_push(CPU_6502* cpu, uint8_t value);
uint8_t cpu_pull(CPU_6502* cpu);
void cpu_push_word(CPU_6502* cpu, uint16_t value);
uint16_t cpu_pull_word(CPU_6502* cpu);

// Status flag operations
void cpu_set_flag(CPU_6502* cpu, uint8_t flag);
void cpu_clear_flag(CPU_6502* cpu, uint8_t flag);
bool cpu_get_flag(CPU_6502* cpu, uint8_t flag);
void cpu_set_zero_negative(CPU_6502* cpu, uint8_t value);

// Addressing mode functions
uint16_t cpu_get_operand_address(CPU_6502* cpu, CPU_AddressingMode mode);

// Instruction set (to be implemented in cpu.c)
extern CPU_Instruction instruction_set[256];

// Debug functions
void cpu_print_state(CPU_6502* cpu);
void cpu_print_memory(CPU_6502* cpu, uint16_t start, uint16_t end);

// CPU test functions
void run_cpu_tests(void);
void cpu_basic_test(uint8_t* program, uint16_t size, uint16_t start_addr, uint8_t expected_ac, uint8_t expected_x, uint8_t expected_y);
