#include <stdio.h>
#include "util/io.h"
#include "util/error.h"
#include "lexer/lexer.h"

int main() {

	char *file_path = "../test.cade";
	File source_code = io_file_read(file_path);		

	if (!source_code.is_valid) {
		ERROR_EXIT("Error reading in file: %s\n", file_path);	
	}

	run_lexer_tests(source_code.data);



	return 0;
}	
