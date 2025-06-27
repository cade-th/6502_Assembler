#include "evaluator.h"
#include "../util/dyn_array.h"
#include <stdio.h>

Evaluator eval_new(Instruction *input) {
	Evaluator eval;
	eval.input = input;
	return eval;
}

static void serialize_instruction(unsigned char *out, Instruction instr) {
    out[0] = (unsigned char) instr.opcode;
    
    if (instr.mode == IMMEDIATE) {
        out[1] = (unsigned char) instr.operand;
    }
}

eval_error evaluate(Evaluator *self) {
	eval_error error = {0}; // Initialize to zero
	const char *out_path = "simple.hex";
	FILE *file = fopen(out_path, "wb");
	if (!file) {
		perror("Failed to open output file");
		error.ok = false;
		error.type = UNKNOWN;
		return error;
	}

	int num_instructions = 1;

	for (int i = 0; i < num_instructions; i++) {
		unsigned char buffer[2]; // Most 6502 instructions are 1-3 bytes
		serialize_instruction(buffer, self->input[i]);
		
		fwrite(&buffer[0], 1, 1, file);
		
		if (self->input[i].mode == IMMEDIATE) {
			fwrite(&buffer[1], 1, 1, file);
		}
	}
	fclose(file);
	
	// Set success
	error.ok = true;
	error.type = EVAL_BODY; // Set a valid type for success case
	return error;
}

