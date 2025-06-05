#include "../include/dyn_array_tests.h"
#include <string.h>

/*
void test_array_push(void) {
    
    typedef enum {
        INT,
        PLUS
    }Token_t;

    typedef struct {
        char *literal;
        Token_t type;
    } Token;

    Token *tokens = DYN_ARRAY(Token);

    Token token_1 = (Token) {
        "1", 
        INT
    };
    
    Token token_2 = (Token) {
        "+", 
        PLUS
    };

    ARRAY_PUSH(tokens, token_1);
    ARRAY_PUSH(tokens, token_2);

    TEST_ASSERT(tokens[0].type == INT);
    TEST_ASSERT(tokens[1].type == PLUS);

    TEST_ASSERT((tokens + 0)->type == INT);
    TEST_ASSERT((tokens + 1)->type == PLUS);

    Token token_3 = (Token) {
        "2",
        INT
    };

    ARRAY_PUSH(tokens, token_3);

    TEST_ASSERT(tokens[2].type == INT);
    TEST_ASSERT(strcmp(tokens[2].literal, "2") == 0);

    TEST_ASSERT(ARRAY_LENGTH(tokens) == 3);

}

*/
