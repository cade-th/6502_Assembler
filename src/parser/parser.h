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

#define OPCODE_LIST		\
	X(LDA, 0xA9)		\
	X(LDX, 0xA2)		\
	X(LDY, 0xA0)		\
	X(STA, 0x8D)		\
	X(STY, 0x8C)			

typedef enum {
#define X(op, val) op = val,
	OPCODE_LIST
#undef X
} OPCODE;

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
void parser_basic_test(Token *input, Instruction *expected_instructions, int expected_count);
