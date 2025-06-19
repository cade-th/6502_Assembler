#include "parser.h"
#include <stdio.h>


static bool instru_is_equal(parser_error *error, Instruction one, Instruction two) {
    if (one.mode != two.mode) {
	return false;
    }
    if (one.opcode != two.opcode) {
	return false;
    }
    if (one.operand != two.operand) {
	return false;
    }
    if (one.is_hex != two.is_hex) {
	return false;
    }
    return true;
}


void run_parser_tests() {
    Token input_1[] = {
	{IDENT, "LDA"},
	{HASH, "#"},
	{DOLLAR, "$"},
	{INT, "10"},
	{Eof, " "}
    };
    
    parser_basic_test(input_1);

}

void parser_basic_test(Token *input) {

    parser_error error;

    Instruction expected_instructions[] = {
	{IMMEDIATE, LDA, 10, true},
    };

    Parser parser = parser_new(input);

    error = parse(&parser);

    Instruction *result_instructions = error.data;

    if (error.ok) {

	if (instru_is_equal(&error, result_instructions[0], expected_instructions[0])) {
	    printf("Parser Test: PASS\n");
	    return;
	}
    }
    printf("Parser Test: FAIL\n");
    switch (error.type) {
	case PARSER_BODY:
	    printf("Write the parse function\n");
	    break;
    };
}

