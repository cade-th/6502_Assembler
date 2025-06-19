#pragma once
#include "../lexer/lexer.h"
#include "../util/error.h"
/*
Examples of directives:
.org or * = sets the starting memory address for code or data   
Labels are kind of like directives IG
MyLabel: used to define addresses that can be jumped to or referenced
*/

typedef enum {
	PARSER_BODY,
	OK,
} Parser_Error_t;
ERROR(Parser_Error_t) parser_error;

typedef enum {
    IMMEDIATE = 0,              // Immediate Addressing Mode (LDA #$10)
    ZERO_PAGE,                  // Zero-Page Addressing Mode (LDA $10)
    ABSOLUTE,                   // Absolute Addressing Mode (LDA $1000)
    ZERO_PAGE_X_INDEXED,        // Zero-Page X-Indexed Addressing Mode (LDA $10, X)
    ZERO_PAGE_Y_INDEXED,        // Zero-Page Y-Indexed Addressing Mode (LDA $10, Y)
    ABSOLUTE_X_INDEXED,         // Absolute X-Indexed Addressing Mode (LDA $1000, X)
    ABSOLUTE_Y_INDEXED,         // Absolute Y-Indexed Addressing Mode (LDA $1000, Y)
    INDIRECT,                   // Indirect Addressing Mode (LDA ($1000))
    INDEXED_INDIRECT,           // Indexed Indirect Addressing Mode (LDA ($10, X))
    INDIRECT_INDEXED,           // Indirect Indexed Addressing Mode (LDA ($10), Y)
    ACCUMULATOR,                // Accumulator Addressing Mode (TAY)
    IMPLIED                     // Implied Addressing Mode (CLC)
} Addressing_Mode;

// idk about the hex values ATM
typedef enum {
    // Load Instructions
    LDA = 0xA9,                // LDA (Load Accumulator)
    LDX = 0xA2,                // LDX (Load X Register)
    LDY = 0xA0,                // LDY (Load Y Register)

    // Store Instructions
    STA = 0x8D,                // STA (Store Accumulator)
    STX = 0x8E,                // STX (Store X Register)
    STY = 0x8C,                // STY (Store Y Register)

    // Transfer Instructions
    TAX = 0xAA,                // TAX (Transfer Accumulator to X)
    TAY = 0xA8,                // TAY (Transfer Accumulator to Y)
    TXA = 0x8A,                // TXA (Transfer X to Accumulator)
    TYA = 0x98,                // TYA (Transfer Y to Accumulator)
    TSX = 0xBA,                // TSX (Transfer Stack Pointer to X)
    TXS = 0x9A,                // TXS (Transfer X to Stack Pointer)

    // Arithmetic and Logic Operations
    ADC = 0x69,                // ADC (Add with Carry)
    SBC = 0xE9,                // SBC (Subtract with Carry)
    AND = 0x29,                // AND (Logical AND)
    ORA = 0x09,                // ORA (Logical OR)
    EOR = 0x49,                // EOR (Exclusive OR)
    BIT = 0x24,                // BIT (Test Bits)
    CMP = 0xC9,                // CMP (Compare)
    CPX = 0xE0,                // CPX (Compare X)
    CPY = 0xC0,                // CPY (Compare Y)

    // Increment and Decrement
    INX = 0xE8,                // INX (Increment X)
    INY = 0xC8,                // INY (Increment Y)
    DEC = 0xC6,                // DEC (Decrement)
    DEX = 0xCA,                // DEX (Decrement X)
    DEY = 0x88,                // DEY (Decrement Y)
    INC = 0xE6,                // INC (Increment)

    // Shift and Rotate
    ASL = 0x0A,                // ASL (Arithmetic Shift Left)
    LSR = 0x4A,                // LSR (Logical Shift Right)
    ROL = 0x2A,                // ROL (Rotate Left)
    ROR = 0x6A,                // ROR (Rotate Right)

    // Stack Instructions
    PHA = 0x48,                // PHA (Push Accumulator onto Stack)
    PHP = 0x08,                // PHP (Push Processor Status onto Stack)
    PLA = 0x68,                // PLA (Pull Accumulator from Stack)
    PLP = 0x28,                // PLP (Pull Processor Status from Stack)

    // Branch Instructions
    BCC = 0x90,                // BCC (Branch if Carry Clear)
    BCS = 0xB0,                // BCS (Branch if Carry Set)
    BEQ = 0xF0,                // BEQ (Branch if Equal)
    BMI = 0x30,                // BMI (Branch if Minus)
    BNE = 0xD0,                // BNE (Branch if Not Equal)
    BPL = 0x10,                // BPL (Branch if Plus)
    BRK = 0x00,                // BRK (Force Break)
    BVS = 0x70,                // BVS (Branch if Overflow Set)
    BVC = 0x50,                // BVC (Branch if Overflow Clear)

    // Control Instructions
    CLC = 0x18,                // CLC (Clear Carry Flag)
    CLD = 0xD8,                // CLD (Clear Decimal Mode)
    CLI = 0x58,                // CLI (Clear Interrupt Disable)
    CLV = 0xB8,                // CLV (Clear Overflow Flag)
    SEC = 0x38,                // SEC (Set Carry Flag)
    SED = 0xF8,                // SED (Set Decimal Mode)
    SEI = 0x78,                // SEI (Set Interrupt Disable)

    // NOP (No Operation)
    NOP = 0xEA                 // NOP (No Operation)

} OPCODE;

typedef struct {
	Addressing_Mode mode;	
	OPCODE opcode;
	int operand;
	bool is_hex;
} Instruction;

typedef enum {
	LABEL_N,
	DIRECTIVE,
	INSTRUCTION,
} Node_t;

typedef struct Node { 		 
	Node_t type;  		
	char *name;
	void *children;
} Node;               		



typedef struct {
	Token *input;	
	Node *program;
	Node *current_node;
	Token *current_token;
	int instruction_count;
	int tree_depth;
} Parser;

// Node types
// This should return a pointer to the root node called "program"
// the input is a malloced dynamic array of tokens
parser_error parse(Parser *self);
Parser parser_new(Token *input);

void run_parser_tests();
void parser_basic_test(Token *input);
