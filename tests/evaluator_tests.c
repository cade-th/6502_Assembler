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

	evaluator_basic_test(input_1);
}

void evaluator_basic_test(Instruction *input) {

	eval_error error; 
	Evaluator eval = eval_new(input);
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


