#include "lexer.h"	
#include <stdio.h>
#include <string.h>
#include "../util/dyn_array.h"

/*
    Source code simple:

    LDA #$10

*/

/*
    Source code less simple:

    LDX #0             ; X = 0 (index of Fibonacci)
    LDY #1             ; Y = 1 (F(1))
    LDA #0             ; A = 0 (F(0))
    STA $0202          ; Store F(0) (low byte) in memory location $0202
    STY $0201          ; Store F(1) (high byte) in memory location $0201
*/

void run_lexer_tests(char *source_code_simple, char *source_code_less_simple) {
	Token expected_tokens_1[] = {
            {IDENT, "LDA"},
            {HASH, "#"},
            {DOLLAR, "$"},
            {INT, "10"},
            {Eof, " "}
	};

        Token expected_tokens_2[] = {
            {IDENT, "LDX"},     
            {HASH, "#"},        
            {INT, "0"},         
            {IDENT, "LDY"},     
            {HASH, "#"},        
            {INT, "1"},         
            {IDENT, "LDA"},     
            {HASH, "#"},        
            {INT, "0"},         
            {IDENT, "STA"},     
            {DOLLAR, "$"},      
            {INT, "0202"},      
            {IDENT, "STY"},     
            {DOLLAR, "$"},      
            {INT, "0201"},      
            {Eof, " "}          
        };

	lexer_basic_test(source_code_simple, 5, expected_tokens_1);
	lexer_basic_test(source_code_less_simple,16, expected_tokens_2);
	}

void lexer_basic_test(char *input_1, int expected_tokens_size, Token expected_tokens[]) {
    lexer_error error;

    Lexer lexer = Lexer_new(input_1);
    error = lex(&lexer);    
    int num_tokens;
    Token *tokens = (Token*)error.data;
    num_tokens = ARRAY_LENGTH(tokens);
    printf("Number of tokens to be checked: %d\n", num_tokens);

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
