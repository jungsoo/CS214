#ifndef TOKENIZER_H
#define TOKENIZER_H

struct TokenizerT_ {
	char* copied_string;
	char* current_position;
};

typedef struct TokenizerT_ TokenizerT;

// Helper functions
char *unescape_string(char *);
char is_delimiter(char);
char is_escape_character(char);
int char_to_hex(char);
int char_to_oct(char);

// Tokenizer methods
TokenizerT *TKCreate(char *);
char *TKGetNextToken(TokenizerT *);
void TKDestroy(TokenizerT *);

#endif
