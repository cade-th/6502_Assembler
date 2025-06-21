#pragma once
#include "../parser/parser.h"
#include "../util/error.h"

typedef enum {
	EVAL_BODY,
	UNKNOWN,
	SUCCESS,
} Evaluator_Error_t;
ERROR(Evaluator_Error_t) eval_error;

typedef struct {
	int program_counter;
	Instruction *input;
} Evaluator;

// Evaluate should return an array of instructions that the generator than loops through and writes the binary
eval_error evaluate(Evaluator *self);
Evaluator eval_new(Instruction *input);
void generate_binary();


void run_evaluator_tests();
void evaluator_basic_test(Instruction *input);

int run_end_to_end_tests();
