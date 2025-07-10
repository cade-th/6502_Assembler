#include "cpu.h"
#include <stdio.h>
#include <string.h>

// Initialize CPU
CPU_6502 cpu_init(void) {
    CPU_6502 cpu;
    memset(&cpu, 0, sizeof(CPU_6502));
    
    // Initialize registers to known state
    cpu.ac = 0;
    cpu.x = 0;
    cpu.y = 0;
    cpu.sp = 0xFF;  // Stack pointer starts at top of stack
    cpu.pc = 0;
    cpu.sr = FLAG_UNUSED;  // Unused flag is always set
    cpu.cycles = 0;
    cpu.halted = false;
    
    // Clear memory
    memset(cpu.memory, 0, MEMORY_SIZE);
    
    return cpu;
}

// Reset CPU to initial state
void cpu_reset(CPU_6502* cpu) {
    cpu->ac = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFF;
    cpu->pc = cpu_read_word(cpu, 0xFFFC);  // Reset vector at $FFFC
    cpu->sr = FLAG_UNUSED;
    cpu->cycles = 0;
    cpu->halted = false;
}

// Load program into memory
void cpu_load_program(CPU_6502* cpu, uint8_t* program, uint16_t size, uint16_t start_addr) {
    memcpy(&cpu->memory[start_addr], program, size);
    cpu->pc = start_addr;
}

// Fetch single byte from current PC location
uint8_t cpu_fetch(CPU_6502* cpu) {
    uint8_t data = cpu->memory[cpu->pc];
    cpu->pc++;
    return data;
}

// Fetch 16-bit word from current PC location (little-endian)
uint16_t cpu_fetch_word(CPU_6502* cpu) {
    uint16_t low = cpu_fetch(cpu);
    uint16_t high = cpu_fetch(cpu);
    return (high << 8) | low;
}

// Memory access functions
uint8_t cpu_read(CPU_6502* cpu, uint16_t address) {
    return cpu->memory[address];
}

void cpu_write(CPU_6502* cpu, uint16_t address, uint8_t value) {
    cpu->memory[address] = value;
}

uint16_t cpu_read_word(CPU_6502* cpu, uint16_t address) {
    uint16_t low = cpu_read(cpu, address);
    uint16_t high = cpu_read(cpu, address + 1);
    return (high << 8) | low;
}

void cpu_write_word(CPU_6502* cpu, uint16_t address, uint16_t value) {
    cpu_write(cpu, address, value & 0xFF);
    cpu_write(cpu, address + 1, (value >> 8) & 0xFF);
}

// Stack operations
void cpu_push(CPU_6502* cpu, uint8_t value) {
    cpu_write(cpu, STACK_OFFSET + cpu->sp, value);
    cpu->sp--;
}

uint8_t cpu_pull(CPU_6502* cpu) {
    cpu->sp++;
    return cpu_read(cpu, STACK_OFFSET + cpu->sp);
}

void cpu_push_word(CPU_6502* cpu, uint16_t value) {
    cpu_push(cpu, (value >> 8) & 0xFF);  // High byte first
    cpu_push(cpu, value & 0xFF);         // Low byte second
}

uint16_t cpu_pull_word(CPU_6502* cpu) {
    uint16_t low = cpu_pull(cpu);
    uint16_t high = cpu_pull(cpu);
    return (high << 8) | low;
}

// Status flag operations
void cpu_set_flag(CPU_6502* cpu, uint8_t flag) {
    cpu->sr |= flag;
}

void cpu_clear_flag(CPU_6502* cpu, uint8_t flag) {
    cpu->sr &= ~flag;
}

bool cpu_get_flag(CPU_6502* cpu, uint8_t flag) {
    return (cpu->sr & flag) != 0;
}

void cpu_set_zero_negative(CPU_6502* cpu, uint8_t value) {
    if (value == 0) {
        cpu_set_flag(cpu, FLAG_ZERO);
    } else {
        cpu_clear_flag(cpu, FLAG_ZERO);
    }
    
    if (value & 0x80) {
        cpu_set_flag(cpu, FLAG_NEGATIVE);
    } else {
        cpu_clear_flag(cpu, FLAG_NEGATIVE);
    }
}

// Execute a single instruction
void cpu_execute(CPU_6502* cpu) {
    if (cpu->halted) {
        return;
    }
    
    // Fetch opcode
    uint8_t opcode = cpu_fetch(cpu);
    
    // For now, implement a few basic instructions
    switch (opcode) {
        case 0xA9:  // LDA Immediate
            {
                uint8_t value = cpu_fetch(cpu);
                cpu->ac = value;
                cpu_set_zero_negative(cpu, value);
                cpu->cycles += 2;
            }
            break;
            
        case 0xA2:  // LDX Immediate
            {
                uint8_t value = cpu_fetch(cpu);
                cpu->x = value;
                cpu_set_zero_negative(cpu, value);
                cpu->cycles += 2;
            }
            break;
            
        case 0xA0:  // LDY Immediate
            {
                uint8_t value = cpu_fetch(cpu);
                cpu->y = value;
                cpu_set_zero_negative(cpu, value);
                cpu->cycles += 2;
            }
            break;
            
        case 0x8D:  // STA Absolute
            {
                uint16_t address = cpu_fetch_word(cpu);
                cpu_write(cpu, address, cpu->ac);
                cpu->cycles += 4;
            }
            break;
            
        case 0x8C:  // STY Absolute
            {
                uint16_t address = cpu_fetch_word(cpu);
                cpu_write(cpu, address, cpu->y);
                cpu->cycles += 4;
            }
            break;
            
        case 0x00:  // BRK
            cpu->halted = true;
            cpu->cycles += 7;
            break;
            
        default:
            printf("Unknown opcode: 0x%02X at PC=0x%04X\n", opcode, cpu->pc - 1);
            cpu->halted = true;
            break;
    }
}

// Step the CPU (fetch, decode, execute)
int cpu_step(CPU_6502* cpu) {
    if (cpu->halted) {
        return 0;
    }
    
    int cycles_before = cpu->cycles;
    cpu_execute(cpu);
    return cpu->cycles - cycles_before;
}

// Debug functions
void cpu_print_state(CPU_6502* cpu) {
    printf("PC: $%04X  AC: $%02X  X: $%02X  Y: $%02X  SP: $%02X  SR: $%02X\n",
           cpu->pc, cpu->ac, cpu->x, cpu->y, cpu->sp, cpu->sr);
    printf("Flags: %c%c%c%c%c%c%c%c\n",
           cpu_get_flag(cpu, FLAG_NEGATIVE) ? 'N' : '-',
           cpu_get_flag(cpu, FLAG_OVERFLOW) ? 'V' : '-',
           cpu_get_flag(cpu, FLAG_UNUSED) ? 'U' : '-',
           cpu_get_flag(cpu, FLAG_BREAK) ? 'B' : '-',
           cpu_get_flag(cpu, FLAG_DECIMAL) ? 'D' : '-',
           cpu_get_flag(cpu, FLAG_INTERRUPT) ? 'I' : '-',
           cpu_get_flag(cpu, FLAG_ZERO) ? 'Z' : '-',
           cpu_get_flag(cpu, FLAG_CARRY) ? 'C' : '-');
    printf("Cycles: %d  Halted: %s\n", cpu->cycles, cpu->halted ? "Yes" : "No");
}

void cpu_print_memory(CPU_6502* cpu, uint16_t start, uint16_t end) {
    for (uint16_t addr = start; addr <= end; addr += 16) {
        printf("%04X: ", addr);
        for (int i = 0; i < 16 && addr + i <= end; i++) {
            printf("%02X ", cpu->memory[addr + i]);
        }
        printf("\n");
    }
}
