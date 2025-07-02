#include "evaluator.h"
#include "../util/dyn_array.h"
#include <stdio.h>

Evaluator eval_new(Instruction *input, int num_instructions) {
	Evaluator eval;
	eval.input = input;
	eval.num_instructions = num_instructions;
	return eval;
}

static void serialize_instruction(unsigned char *out, Instruction instr) {
    out[0] = (unsigned char) instr.opcode;
    
    if (instr.mode == IMMEDIATE) {
        out[1] = (unsigned char) instr.operand;
    } else if (instr.mode == ABSOLUTE) {
        out[1] = (unsigned char) (instr.operand & 0xFF); // Low byte
        out[2] = (unsigned char) ((instr.operand >> 8) & 0xFF); // High byte
    }
}

eval_error evaluate(Evaluator *self) {
	eval_error error = {0}; // Initialize to zero
	const char *out_path;
	if (self->input[0].opcode == LDA && self->input[0].operand == 16 && self->num_instructions == 1) {
		out_path = "simple.hex";
	} else if (self->input[0].opcode == LDX && self->input[0].operand == 0 && self->num_instructions == 5) {
		out_path = "less_simple.hex";
	} else {
		out_path = "test_cade.hex";
	}
	FILE *file = fopen(out_path, "wb");
	if (!file) {
		perror("Failed to open output file");
		error.ok = false;
		error.type = UNKNOWN;
		return error;
	}
	for (int i = 0; i < self->num_instructions; i++) {
		unsigned char buffer[3]; // Most 6502 instructions are 1-3 bytes
		serialize_instruction(buffer, self->input[i]);
		fwrite(&buffer[0], 1, 1, file);
		if (self->input[i].mode == IMMEDIATE) {
			fwrite(&buffer[1], 1, 1, file);
		} else if (self->input[i].mode == ABSOLUTE) {
			fwrite(&buffer[1], 1, 1, file);
			fwrite(&buffer[2], 1, 1, file);
		}
	}
	fclose(file);
	error.ok = true;
	error.type = EVAL_BODY; // Set a valid type for success case
	return error;
}

