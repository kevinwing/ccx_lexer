/**
 * @file parse.h
 * @author Kevin Wing (krwing@nic.edu)
 * @brief Declarations file for a Lexical Analyzer.
 * @date 2021-09-10
 */
#ifndef PARSE_H
#define PARSE_H

// Globals
// #define MAX_STR_LEN 256 // max length of output filename.
#define MAX_BUFFER 65536 // max length of file buffer.
// #define NUM_LEXEMES 1
#define PEEK_BUFFER_LEN 16

// #define _POSIX_C_SOURCE 1 // for PATH_MAX to work.

// Standard library includes
#include <stdio.h>
#include <ctype.h> // for type based comparison, i.e. isdigit(), to work
#include <stdlib.h> // for exit() to work
#include <string.h> // for string manipulation, i.e. strcat(), to work
#include <linux/limits.h> // for PATH_MAX to work

enum State {
    COMMENT = 0,
    IDENTIFIER,
    STRING,
    KEYWORD,
    CHAR_LITERAL,
    OPERATOR,
    NUM_LITERAL,
    UNKNOWN,
    // ERROR, // UNKNOWN is used for lexeme errors.
    NORMAL,
};
typedef enum State State;

// Function prototypes
void parse(char *filename); // parse input file

State set_state(FILE *fptr, char ch);

int is_comment_end(char ch, FILE *fptr);
int is_string_end(char ch);

int is_double_op(char ch, char lexeme[]);
int is_keyword(char lexeme[]);
int is_comment(char ch, FILE *fptr);
int is_string(char ch, FILE *fptr);
int is_char_delim(char ch);

int is_whitespace(char ch);
int is_single_op(char ch);
int has_double_op(char ch);
int is_allowed_ident_char(char ch);
int is_op_symbol(char ch);
int is_num_literal_end(char ch, FILE *fptr);
int is_num_lit_char(char ch);

int fpeek(FILE *fptr, char str[], int n);
void init_buffer(char buffer[], int size);

#endif

