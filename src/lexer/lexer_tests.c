#include "lexer.h"	
#include <stdio.h>
#include <string.h>
#include "../util/dyn_array.h"

void run_lexer_tests(char *source_code_complex) {
	char *source_code_simple = "=+(){},;";

	Token expected_tokens_1[] = {
	{ASSIGN, "="},
	{PLUS, "+"},
	{LPAREN, "("},
	{RPAREN, ")"},
	{LBRACKET, "{"},
	{RBRACKET, "}"},
	{COMMA, ","},
	{SEMICOLON, ";"},
	{Eof, " "}
	};

        /*
	Token expected_tokens_2[] = {
	{Eof, " "}             
	};
        */

	lexer_basic_test(source_code_simple, 9, expected_tokens_1);
	// lexer_basic_test(source_code_complex.data, 37, expected_tokens_2);
	}

void lexer_basic_test(char *input_1, int expected_tokens_size, Token expected_tokens[]) {
    lexer_error error;

    Lexer lexer = Lexer_new(input_1);
    error = lex(&lexer);    
    int num_tokens;
    Token *tokens = (Token*)error.data;
    num_tokens = ARRAY_LENGTH(tokens);

    if (error.ok) {
        if (num_tokens != expected_tokens_size) {
            error.ok = false;
            error.type = NUM_TOKENS;
        }
        if (error.ok) {
                for (int i=0; i < expected_tokens_size; i++) {
                    Token current_token = tokens[i];
                    if(current_token.type != expected_tokens[i].type) {
                        error.ok = false;
                        error.type = INCORRECT_TOKEN_TYPE;
                        // make the error data here be an integer somehow
                    }
                    if(strcmp(current_token.literal,expected_tokens[i].literal) != 0) {
                        error.ok = false;
                        error.type = INCORRECT_TOKEN_LITERAL;
                        error.data = current_token.literal;

                    }
                }
                if (error.ok) {
                    printf("Lex Test: PASS\n");
                    return;
                }
        }
    }
    printf("Lex Test: Fail\n");
    switch (error.type) {

        case INVALID_TOKEN:
            printf("Lexer caught an invalid token: %c\n", *(char*)error.data);
            break;
        case INCORRECT_TOKEN_TYPE:
            printf("Incorrect Token Type: ?\n");
            break;
        case INCORRECT_TOKEN_LITERAL:
            printf("Incorrect Token Literal: %s\n", (char *)error.data);
            break;
        case FUNCTION_BODY:
            printf("Write the lexer loser\n");
            break;
        case ILLEGAL_TOKEN:
            printf("Illegal Token\n");
            break;
        case NUM_TOKENS:
            printf("Incorrect number of tokens. Expected Tokens: %d, Processed Tokens %d\n", expected_tokens_size, num_tokens);
            break;
    };


}
