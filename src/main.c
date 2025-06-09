#include <stdio.h>
#include "util/io.h"
#include "util/error.h"
#include "lexer/lexer.h"

int main() {

	char *file_path_simple = "../6502/simple.cade";
	File source_code_simple = io_file_read(file_path_simple);		

	if (!source_code_simple.is_valid) {
		return 1;
	}
	
	char *file_path_not_simple = "../6502/less_simple.cade";
	File source_code_not_simple = io_file_read(file_path_not_simple);		

	if (!source_code_simple.is_valid) {
		return 1;
	}

	run_lexer_tests(source_code_simple.data,source_code_not_simple.data);
	return 0;
}	
