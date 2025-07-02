#pragma once
#include "../parser/parser.h"
#include "../util/error.h"

typedef enum {
	EVAL_BODY,
	UNKNOWN,
} Evaluator_Error_t;
ERROR(Evaluator_Error_t) eval_error;

typedef struct {
	int program_counter;
	Instruction *input;
	int num_instructions;
} Evaluator;

// Evaluate should return an array of instructions that the generator than loops through and writes the binary
eval_error evaluate(Evaluator *self);
Evaluator eval_new(Instruction *input, int num_instructions);
void generate_binary();


void run_evaluator_tests();
void evaluator_simple_test(Instruction *input, int num_instructions);
void evaluator_less_simple_test(Instruction *input, int num_instructions);
void evaluator_test_cade_test(Instruction *input, int num_instructions);

void run_end_to_end_tests();
