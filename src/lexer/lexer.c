#define _POSIX_C_SOURCE 200809L  
#include "lexer.h"
#include "../util/dyn_array.h"
#include <string.h>
#include <ctype.h>

static lexer_error error = { .ok = true}; // Initialize error

// Gives the next character and advance the position in the input string
static void read_char(Lexer *self) {
	if (self->read_position >= (int)strlen(self->input)) {
	        // stop condition for next_token
		self->ch = 0;
	} else {
		self->ch = self->input[self->read_position];
	}
	self->position = self->read_position;
	self->read_position += 1;
}

static bool is_letter(char ch) {
   return isalpha(ch) || ch == '_';
}


static void skip_whitespace(Lexer *self) {
   while (isspace(self->ch)) {
      read_char(self);
   }
}

static void remove_comment(Lexer *self) {
   while(self->ch != '\n' && self->ch != 0) {
      read_char(self);
   }
}

static char *read_identifier(Lexer *self) {
   int position = self->position;
   while (is_letter(self->ch) || isdigit(self->ch)) {
	 read_char(self);
   }
   return strndup(self->input + position, self->position - position);
}

static char *read_number(Lexer *self) {
   int position = self->position;
   while (isdigit(self->ch)) {
      read_char(self);
   }
   return strndup(self->input + position, self->position - position);
}

static Token next_token(Lexer *self) {
    Token tok = (Token){
        Eof,
        " "
    };

    skip_whitespace(self);

    // Do labels next

    switch (self->ch) {
        case '#': tok.type = HASH; tok.literal = strdup("#"); break;
        case '$': tok.type = DOLLAR; tok.literal = strdup("$"); break;
        case ':': tok.type = COLON; tok.literal = strdup(":"); break;
        case '%': tok.type = PERCENT; tok.literal = strdup("%"); break;
        case '|': tok.type = LOGICAL_OR; tok.literal = strdup("|"); break;
        case '.': tok.type = DOT; tok.literal = strdup("."); break;
        case ';': {
            remove_comment(self);
            return next_token(self);
        }
        case 0: tok.type = Eof; tok.literal = strdup(" "); break;

        default:
            if (is_letter(self->ch)) {
                tok.literal = read_identifier(self);
                tok.type = IDENT;
                return tok;
            }
            else if (isdigit(self->ch)) {
                tok.type = INT;
                tok.literal = read_number(self);
                return tok;
            }
            else {
                tok.type = ILLEGAL;
                tok.literal = "";
                error.type = ILLEGAL_TOKEN;
                THROW(error, char, self->ch);
            }
    }

    read_char(self);
    return tok;
}


lexer_error lex(Lexer *self) {
   Token *tokens = DYN_ARRAY(Token);
   bool should_lex = true;

   while (should_lex) {

      Token current_tok = next_token(self);
      if (!error.ok) {
	 break;
      }
      ARRAY_PUSH(tokens, current_tok);
      if (current_tok.type == Eof) {
	 error.data = tokens;
	 should_lex = false;
      }

   }
   return error;
}

Lexer Lexer_new(char *input) {
   Lexer lexer = {
		.input = input,
		.position = 0,
		.read_position = 0,
		.ch = 0,
	};
	read_char(&lexer);
	return lexer; 
}
