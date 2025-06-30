#include "../src/parser/parser.h"
#include "../src/util/dyn_array.h"
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
    

    Token input_2[] = {
	{IDENT, "LDX"}, {HASH, "#"}, {INT, "0"},         
	{IDENT, "LDY"}, {HASH, "#"}, {INT, "1"},         
	{IDENT, "LDA"}, {HASH, "#"}, {INT, "0"},         
	{IDENT, "STA"}, {DOLLAR, "$"}, {INT, "0202"},      
	{IDENT, "STY"}, {DOLLAR, "$"}, {INT, "0201"},      
	{Eof, " "}          
    };

    Instruction expected_instructions_1[] = {
	{IMMEDIATE, LDA, 16, true},
    };
    Instruction expected_instructions_2[] = {
	{IMMEDIATE, LDX, 0, false},
	{IMMEDIATE, LDY, 1, false},
	{IMMEDIATE, LDA, 0, false},
	{ABSOLUTE, STA, 0x202, true},
	{ABSOLUTE, STY, 0x201, true},
    };

    parser_basic_test(input_1, expected_instructions_1, 1);
    parser_basic_test(input_2, expected_instructions_2, 5);

}

void parser_basic_test(Token *input, Instruction *expected_instructions, int expected_count) {

    parser_error error;

    
    Parser parser = parser_new(input);

    error = parse(&parser);

    Instruction *result_instructions = error.data;

    if (error.ok) {
        // Check if we have the right number of instructions
        int result_count = ARRAY_LENGTH(result_instructions);
        if (result_count != expected_count) {
            printf("Parser Test: FAIL - Expected %d instructions, got %d\n", expected_count, result_count);
            return;
        }
        
        // Check each instruction
        for (int i = 0; i < expected_count; i++) {
            if (!instru_is_equal(&error, result_instructions[i], expected_instructions[i])) {
                printf("Parser Test: FAIL - Instruction %d mismatch\n", i);
                return;
            }
        }
        
        printf("Parser Test: PASS\n");
        return;
    }
    printf("Parser Test: FAIL\n");
    switch (error.type) {
	case PARSER_BODY:
	    printf("Write the parse function\n");
	    break;
    };
}

