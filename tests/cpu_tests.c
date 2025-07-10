#include "../src/emu/cpu.h"
#include <stdio.h>
#include <string.h>

void run_cpu_tests() {
    // Test 1: LDA #$10 (simple.hex equivalent)
    uint8_t program1[] = {0xA9, 0x10};  // LDA #$10
    cpu_basic_test(program1, 2, 0x0000, 0x10, 0x00, 0x00);

    // Test 2: LDX #0, LDY #1, LDA #0 (less_simple.hex equivalent)
    uint8_t program2[] = {0xA2, 0x00, 0xA0, 0x01, 0xA9, 0x00};  // LDX #0, LDY #1, LDA #0
    cpu_basic_test(program2, 6, 0x0000, 0x00, 0x00, 0x01);

    // Test 3: LDA #$10, STA $0200 (store to memory)
    uint8_t program3[] = {0xA9, 0x10, 0x8D, 0x00, 0x02};  // LDA #$10, STA $0200
    cpu_basic_test(program3, 5, 0x0000, 0x10, 0x00, 0x00);
}

void cpu_basic_test(uint8_t* program, uint16_t size, uint16_t start_addr, uint8_t expected_ac, uint8_t expected_x, uint8_t expected_y) {
    cpu_error error = {0}; // Initialize error

    // Initialize CPU
    CPU_6502 cpu = cpu_init();
    
    // Load program
    cpu_load_program(&cpu, program, size, start_addr);
    
    // Execute program step by step until halted
    while (!cpu.halted) {
        int cycles = cpu_step(&cpu);
        if (cycles == 0) {
            // CPU halted or error occurred
            break;
        }
    }
    
    // Check if CPU halted normally (not due to error)
    if (cpu.halted) {
        // Check register values
        if (cpu.ac != expected_ac) {
            error.ok = false;
            error.type = CPU_REGISTER_ERROR;
            THROW(error, uint8_t, cpu.ac);
        }
        
        if (cpu.x != expected_x) {
            error.ok = false;
            error.type = CPU_REGISTER_ERROR;
            THROW(error, uint8_t, cpu.x);
        }
        
        if (cpu.y != expected_y) {
            error.ok = false;
            error.type = CPU_REGISTER_ERROR;
            THROW(error, uint8_t, cpu.y);
        }
        
        // If we get here, test passed
        if (error.ok) {
            printf("CPU Test: PASS\n");
            return;
        }
    } else {
        // CPU didn't halt properly
        error.ok = false;
        error.type = CPU_HALTED;
    }
    
    // Test failed
    printf("CPU Test: FAIL\n");
    switch (error.type) {
        case CPU_FUNCTION_BODY:
            printf("Write the CPU emulator\n");
            break;
        case CPU_INVALID_OPCODE:
            printf("Invalid opcode encountered\n");
            break;
        case CPU_INVALID_ADDRESS:
            printf("Invalid memory address accessed\n");
            break;
        case CPU_HALTED:
            printf("CPU did not halt properly\n");
            break;
        case CPU_MEMORY_ERROR:
            printf("Memory access error\n");
            break;
        case CPU_REGISTER_ERROR:
            printf("Register value mismatch. Expected AC: 0x%02X, X: 0x%02X, Y: 0x%02X. Got AC: 0x%02X, X: 0x%02X, Y: 0x%02X\n", 
                   expected_ac, expected_x, expected_y, cpu.ac, cpu.x, cpu.y);
            break;
    }
} 