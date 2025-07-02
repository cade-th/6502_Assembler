#include "../src/evaluator/evaluator.h"
#include <stdio.h>
#include <string.h> // for memcmp


static bool binary_is_equal(const char *file1_path, const char *file2_path) {
	FILE *file1 = fopen(file1_path, "rb");
	FILE *file2 = fopen(file2_path, "rb");

	if (!file1 || !file2) {
		perror("Failed to open one of the binary files for comparison");
		if (file1) fclose(file1);
		if (file2) fclose(file2);
		return false;
	}

	unsigned char buffer1[256];
	unsigned char buffer2[256];

	size_t read1 = fread(buffer1, 1, sizeof(buffer1), file1);
	size_t read2 = fread(buffer2, 1, sizeof(buffer2), file2);

	fclose(file1);
	fclose(file2);

	if (read1 != read2) {
		printf("Binary file length mismatch: %zu vs %zu\n", read1, read2);
		return false;
	}

	return memcmp(buffer1, buffer2, read1) == 0;
}

void run_evaluator_tests() {
	Instruction input_1[] = {
		{ IMMEDIATE, LDA, 16, true },
	};
	evaluator_simple_test(input_1, sizeof(input_1)/sizeof(Instruction));

	// less_simple.cade instructions
	Instruction less_simple_instructions[] = {
		{ IMMEDIATE, LDX, 0, true },
		{ IMMEDIATE, LDY, 1, true },
		{ IMMEDIATE, LDA, 0, true },
		{ ABSOLUTE,  STA, 0x0202, true },
		{ ABSOLUTE,  STY, 0x0201, true },
	};
	evaluator_less_simple_test(less_simple_instructions, sizeof(less_simple_instructions)/sizeof(Instruction));

	// Test for test.cade - first few instructions
	Instruction test_cade_instructions[] = {
		{ IMMEDIATE, LDX, 0, false },
		{ IMMEDIATE, LDY, 1, false },
		{ IMMEDIATE, LDA, 0, false },
	};
	evaluator_test_cade_test(test_cade_instructions, sizeof(test_cade_instructions)/sizeof(Instruction));
}

void evaluator_simple_test(Instruction *input, int num_instructions) {
	eval_error error;
	Evaluator eval = eval_new(input, num_instructions);
	error = evaluate(&eval);

	const char *actual_path = "simple.hex";
	const char *expected_path = "../6502/simple.hex";

	if (error.ok && binary_is_equal(actual_path, expected_path)) {
		printf("Evaluator Test: PASS\n");
		return;
	} else {
		printf("Evaluator Test: FAIL\n");

		switch (error.type) {
			case EVAL_BODY:
				printf("Write the evaluator\n");
				break;
			case UNKNOWN:
				printf("Probably file IO error\n");
				break;
			default:
				printf("Unknown error\n");
				break;
		}
	}
	return;
}

void evaluator_less_simple_test(Instruction *input, int num_instructions) {
	eval_error error;
	Evaluator eval = eval_new(input, num_instructions);
	error = evaluate(&eval);

	const char *actual_path = "less_simple.hex";
	const char *expected_path = "../6502/less_simple.hex";

	if (error.ok && binary_is_equal(actual_path, expected_path)) {
		printf("Evaluator Test: PASS\n");
		return;
	} else {
		printf("Evaluator Test: FAIL\n");

		switch (error.type) {
			case EVAL_BODY:
				printf("Write the evaluator\n");
				break;
			case UNKNOWN:
				printf("Probably file IO error\n");
				break;
			default:
				printf("Unknown error\n");
				break;
		}
	}
	return;
}

void evaluator_test_cade_test(Instruction *input, int num_instructions) {
	eval_error error;
	Evaluator eval = eval_new(input, num_instructions);
	error = evaluate(&eval);

	const char *actual_path = "test_cade.hex";
	const char *expected_path = "../6502/test_cade.hex";

	if (error.ok && binary_is_equal(actual_path, expected_path)) {
		printf("Evaluator Test: PASS\n");
		return;
	} else {
		printf("Evaluator Test: FAIL\n");
		// ... error handling ...
	}
	return;
}

