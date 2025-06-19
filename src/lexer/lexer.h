#pragma once
#include "../util/error.h"

typedef enum {
	FUNCTION_BODY,	
	NUM_TOKENS,
	INCORRECT_TOKEN_TYPE,
	INCORRECT_TOKEN_LITERAL,
	INVALID_TOKEN,
	ILLEGAL_TOKEN,
} lexer_error_t;
ERROR(lexer_error_t) lexer_error;

typedef enum {
	 // Identifiers and literals
	IDENT,  // labels, variables
	INT,    // 1234567890
	HEX,    // $1234
	
	// Delimiters
	COMMA,     // ,
	SEMICOLON, // ;
	COLON, // :
	LPAREN,    // (
	RPAREN,    // )
	HASH,      // # (immediate mode)
	DOLLAR,    // $ (hex numbers)
	ASSIGN,    // =
	DOT,       // .
	PERCENT,   // %
	LOGICAL_OR, // |
	
	// 6502 Specific
	LABEL,
	COMMENT,
	ILLEGAL,
	Eof,
} Token_t;

typedef struct {
	Token_t type;
	char *literal;
} Token;

typedef struct {
	char *input;
	int position; // current position in input (points to the current char)
	int read_position; // current reading position in the input (after current char)
	char ch; // current char under examination
} Lexer;

Lexer Lexer_new(char *input);
lexer_error lex(Lexer *self);

// lexer test functions
void lexer_basic_test(char *input, int num_tokens, Token expected_tokens[]);
void run_lexer_tests(char *source_code_simple, char *source_code_less_simple);
