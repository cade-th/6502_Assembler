#include "parser.h"
#include "../lexer/lexer.h"
#include "../util/dyn_array.h"
#include <string.h>
#include <stdlib.h>

static parser_error error = { .ok = true};

static OPCODE lookup_opcode(char *opcode_string) {
    OPCODE return_code;
    return return_code;
}

static void parse_instruction(Instruction *current_instruction, Parser *self) {
    if (strcmp(self->current_token->literal, "LDA") == 0) {
	current_instruction->opcode = LDA;	
    }
    self->current_token++;
    if (self->current_token->type == HASH) {
	current_instruction->mode = IMMEDIATE;
    }
    self->current_token++;
    if (self->current_token->type == DOLLAR) {
	current_instruction->is_hex = true;
    }
    self->current_token++;
    if (current_instruction->is_hex) {
        current_instruction->operand = (int)strtol(self->current_token->literal, NULL, 16);
    } else {
        current_instruction->operand = atoi(self->current_token->literal);
    }
    self->current_token++;
}

parser_error parse(Parser *self) {

    Instruction *instructions = DYN_ARRAY(Instruction);

    while (self->current_token->type != Eof) {
	Instruction current_instruction;
	switch (self->current_token->type) {
	    case IDENT:
		parse_instruction(&current_instruction, self);
		break;
	    // Add support for macros here soon
	    default : {
	    };
	};
	ARRAY_PUSH(instructions, current_instruction);
    }

    error.type = OK;
    error.ok = true;
    error.data = instructions;
    return error;
}

Parser parser_new(Token *input) {
    Parser parser;
    parser.input = input;
    parser.current_token = &input[0];
    /*
    parser.program = NULL;
    parser.current_node = NULL;
    parser.instruction_count = 0;
    parser.tree_depth = 0;
    */
    return parser;
}
