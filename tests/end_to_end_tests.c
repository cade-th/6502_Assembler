#include "../src/util/io.h"
#include "../src/lexer/lexer.h"
#include "../src/parser/parser.h"
#include "../src/evaluator/evaluator.h"
#include <stdio.h>
#include <string.h>

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


void run_end_to_end_tests() {

	lexer_error lex_error = { 0 };
	parser_error parse_error = { 0 };
	eval_error eval_error = { 0 };

	char *file_path_simple = "../6502/simple.cade";
	File source_code_simple = io_file_read(file_path_simple);		

	if (!source_code_simple.is_valid) {
		return;
	}
	
	Lexer lexer = Lexer_new(source_code_simple.data);
	lex_error = lex(&lexer);

	Parser parser = parser_new(lex_error.data);
	parse_error = parse(&parser);

	Evaluator eval = eval_new(parse_error.data);
	eval_error = evaluate(&eval);

	const char *actual_path = "simple.hex";
	const char *expected_path = "../6502/simple.hex";

	if (eval_error.ok && binary_is_equal(actual_path, expected_path) ) {
		printf("End to End Test: PASS\n");
		 return;
	}

	switch (eval_error.type) {
		case EVAL_BODY:
			printf("Write this test\n");
			break;
		case UNKNOWN:
			printf("Unknown error\n");
			break;
	}

	return;



}	
