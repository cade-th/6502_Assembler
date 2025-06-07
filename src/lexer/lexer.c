#include "lexer.h"
#include "../util/dyn_array.h"

static lexer_error error = { .ok = true}; // Initialize error

Lexer Lexer_new(char *input) {
   Lexer lexer = {
		.input = input,
		.position = 0,
		.read_position = 0,
		.ch = 0,
	};
	// read_char(&lexer);
	return lexer; 
}
lexer_error lex(Lexer *self) {
    Token *tokens = DYN_ARRAY(Token);
    error.ok = false;
    error.type = FUNCTION_BODY;
    error.data = tokens;
    return error;
}


