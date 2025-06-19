#include "util/io.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "evaluator/evaluator.h"

int main() {

	lexer_error lex_error;
	parser_error parse_error;
	eval_error eval_error;

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

	Lexer lexer = Lexer_new(source_code_simple.data);
	lex_error = lex(&lexer);

	Parser parser = parser_new(lex_error.data);
	parse_error = parse(&parser);

	Evaluator eval = eval_new(parse_error.data);
	evaluate(&eval);

	return 0;
}	
