#include "parser.h"
#include "../lexer/lexer.h"
#include "../util/dyn_array.h"
#include <string.h>
#include <stdlib.h>

static parser_error error = { .ok = true};

static OPCODE lookup_opcode(char *opcode_string) {
#define X(op, val) if (strcmp(opcode_string, #op) == 0) return op;
    OPCODE_LIST
#undef X
    return 0; // Invalid opcode
}

static void parse_instruction(Instruction *current_instruction, Parser *self) {
    // Initialize the instruction with default values
    current_instruction->opcode = 0;
    current_instruction->mode = IMMEDIATE;
    current_instruction->operand = 0;
    current_instruction->is_hex = false;

    // Check if current_token is valid
    if (!self->current_token || !self->current_token->literal) {
        error.ok = false;
        error.type = PARSER_BODY;
        return;
    }

    // Look up the opcode using the x macro
    current_instruction->opcode = lookup_opcode(self->current_token->literal);
    if (current_instruction->opcode == 0) {
        error.ok = false;
        error.type = PARSER_BODY;
        return;
    }
    self->current_token++;

    // Check for immediate mode
    bool is_immediate = false;
    if (self->current_token && self->current_token->type == HASH) {
        current_instruction->mode = IMMEDIATE;
        is_immediate = true;
        self->current_token++;
    }

    // Check for hex mode
    if (self->current_token && self->current_token->type == DOLLAR) {
        current_instruction->is_hex = true;
        if (is_immediate) {
            // Keep mode as IMMEDIATE for #$10
            current_instruction->mode = IMMEDIATE;
        } else {
            current_instruction->mode = ABSOLUTE; // Set mode to ABSOLUTE for $ addressing
        }
        self->current_token++;
    }

    // Now expect an INT
    if (self->current_token && self->current_token->type == INT) {
        if (current_instruction->is_hex) {
            current_instruction->operand = (int)strtol(self->current_token->literal, NULL, 16);
        } else {
            current_instruction->operand = atoi(self->current_token->literal);
        }
        self->current_token++;
    }
}

parser_error parse(Parser *self) {
    // Check if parser and input are valid
    if (!self || !self->input || !self->current_token) {
        error.ok = false;
        error.type = PARSER_BODY;
        return error;
    }

    Instruction *instructions = DYN_ARRAY(Instruction);

    while (self->current_token && self->current_token->type != Eof) {
        Instruction current_instruction;
        
        // Initialize current_instruction with default values
        current_instruction.opcode = 0;
        current_instruction.mode = IMMEDIATE;
        current_instruction.operand = 0;
        current_instruction.is_hex = false;
        
        switch (self->current_token->type) {
            case IDENT:
                parse_instruction(&current_instruction, self);
                break;
            default:
                // Skip unknown tokens
                self->current_token++;
                continue;
        };
        
        // Only push if parsing was successful and opcode is valid
        if (error.ok && current_instruction.opcode != 0) {
            ARRAY_PUSH(instructions, current_instruction);
        }
    }

    error.type = OK;
    error.ok = true;
    error.data = instructions;
    self->instruction_count = ARRAY_LENGTH(instructions); // Set instruction count
    return error;
}

Parser parser_new(Token *input) {
    Parser parser;
    parser.input = input;
    parser.current_token = input ? &input[0] : NULL;
    /*
    parser.program = NULL;
    parser.current_node = NULL;
    parser.instruction_count = 0;
    parser.tree_depth = 0;
    */
    return parser;
}
