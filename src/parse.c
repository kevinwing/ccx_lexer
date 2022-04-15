/**
 * @file parse.c
 * @author Kevin Wing (krwing@nic.edu)
 * @brief Definitions file for a Lexical Analyzer
 * @date 2021-09-10
 */
#include "parse.h"

// token itentifiers
const char *TOKENS[] = {
    " (comment)",
    " (identifier)",
    " (string)",
    " (keyword)",
    " (character literal)",
    " (operator)",
    " (numeric literal)"
};

/**
 * @brief Perform Lexical Analysis on given filename and write output to
 * new file.
 *
 * @param filename (char *) String containing filename to parse.
 */
void parse(char *filename)
{
    FILE *fin, *fout;
    char *newext = ".lexer.out";
    char foutname[PATH_MAX + 1] = "";

    // build output filename
    strcat(foutname, filename);
    strcat(foutname, newext);

    // check if input filestream opened successfully.
    if ((fin = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file. No such file: %s\n", filename);
        return;
    }

    // check if output file steam opened successfully.
    if ((fout = fopen(foutname, "w")) == NULL)
    {
        fprintf(stderr, "Error creating file: %s\n", foutname);
        return;
    }

    char ch; // current character to evaluate
    char lexeme[MAX_BUFFER]; // buffer to hold current lexeme
    int index = 0; // current index of lexeme
    int success = 0; // boolean for successful lexeme construction
    State state = NORMAL; // to hold current state

    // read each character in fin and end when end of file reached.
    while ((ch = getc(fin)) != EOF)
    {
        // Lexical analysis state machine.
        switch (state)
        {
        case COMMENT: // comment parsing state.
            // if end of comment is detected, wrap up lexeme, set success to 1
            if (is_comment_end(ch, fin))
            {
                lexeme[index++] = ch; // append '*' to lexeme.
                lexeme[index] = getc(fin); // get '/' and append to lexeme.

                success = 1; // set success to true.
                break;
            }

            // append current character to lexeme while in COMMENT state.
            lexeme[index++] = ch;
            break;


        case STRING: // string parsing state.
            if (is_string_end(ch))
            {
                lexeme[index] = ch;

                success = 1;
                break;
            }

            lexeme[index++] = ch;
            break;

        case KEYWORD: // keyword parsing state
            // if ch is digit or underscore, switch state to identifier
            if (isdigit(ch) || ch == '_')
            {
                state = IDENTIFIER;
            }
            // keywords are all alpha, when non-alpha detected, end parsing
            else if (!isalpha(ch))
            {
                // check if collected lexeme is reserved, else it's an identifier
                if (!is_keyword(lexeme))
                {
                    state = IDENTIFIER;
                }

                // put current character back for next state analysis
                ungetc(ch, fin);
                success = 1; // set succes to 1
                break;
            }

            lexeme[index++] = ch; // append current char to lexeme
            break;

        case IDENTIFIER: // identifier parsing state
            // check if allowed character for identifier, end lexeme if not
            if (!is_allowed_ident_char(ch))
            {
                ungetc(ch, fin); // return current char to stream.
                success = 1; // set success to true
                break;
            }

            lexeme[index++] = ch; // append current ident char to lexeme
            break;

        case OPERATOR: // operator parsing state.
            // check if symbol only is in single operator, i.e., '(', ')', etc.
            if (is_single_op(lexeme[0]))
            {
                ungetc(ch, fin); // return current char to stream
                success = 1;
                break;
            }

            // verify first index is potential double operator
            if (has_double_op(lexeme[0]))
            {
                // check if operator in lexeme + ch is compound
                if (is_double_op(ch, lexeme))
                {
                    lexeme[index] = ch; // second char to compound operator
                }
                else
                {
                    ungetc(ch, fin); // return current char to stream
                }
                success = 1;
                break;
            }
            break;

        case CHAR_LITERAL:
            if (is_char_delim(ch))
            {
                lexeme[index] = ch;
                success = 1;
                break;
            }
            lexeme[index++] = ch;
            break;

        case NUM_LITERAL:
            if (is_num_literal_end(ch, fin))
            {
                ungetc(ch, fin);
                success = 1;
                break;
            }
            lexeme[index++] = ch;
            break;

        case UNKNOWN: // unknown state to handle unknown lexemes
            fprintf(stderr, "Error. Unkown lexeme: %s\n", lexeme);
            exit(1); // end program.
            break;

        case NORMAL: // normal state
            // check for empty lexeme, re-init if not
            if (lexeme[0] != '\0')
            {
                init_buffer(lexeme, MAX_BUFFER);
            }

            state = set_state(fin, ch); // determine new state
            // if new state is other than normal, start building lexeme
            if (state != NORMAL)
            {
                lexeme[index++] = ch;
            }

            break;

        default: // unused default state
            break;
        }

        // check if lexeme is complete and state is not UNKNOWN
        // if both cases are true, append token to lexeme and write to
        // output file, set state to NORMAL and reset control and index vars
        if (success && (state != UNKNOWN))
        {
            // append token type to lexeme.
            strcat(lexeme, TOKENS[state]);
            // append lexeme newline termination.
            strcat(lexeme, "\n");

            // write lexeme to output file
            fputs(lexeme, fout);

            // reset state to NORMAL
            state = NORMAL;
            // reset status vars
            index = 0;
            success = 0;
        }
    }

    // flush and close the file streams.
    fclose(fin);
    fclose(fout);
}

/**
 * @brief read next n chars from fptr and return n characters stored in in str.
 * If n chars read succesfully, return n, else return EOF.
 *
 * @param fptr FILE pointer of current file being parsed
 * @param str A buffer string to load next n chars into.
 * @param n Number of characters to read from file stream
 * @return int Number of characters read or EOF if end of file.
 */
int fpeek(FILE *fptr, char str[], int n)
{
    // init_buffer(str, n);
    for (int i = 0; i < n; ++i)
    {
        int ch = fgetc(fptr);
        if (ch == EOF)
        {
            str[i] = '\0';
            return ch; // return EOF
        }
        str[i] = ch;
    }

    str[n] = '\0';

    for (int i = (n - 1); i >= 0; --i)
    {
        ungetc(str[i], fptr);
    }

    return n;
}

/**
 * @brief Set the current state by analyzing the current character.
 *
 * @param fptr (FILE *) pointer to current input file
 * @param ch (char) current character of input file
 * @return State return new state machine value.
 */
State set_state(FILE *fptr, char ch)
{
    State newState = NORMAL; // set return value to NORMAL by default

    // if char is not a whitespace character, determine lexeme state
    // if (!is_whitespace(ch))
    if (!isspace(ch))
    {
        // detect comment
        if (is_comment(ch, fptr))
        {
            // ungetc(ch, fptr);
            newState = COMMENT;
        }
        // detect string
        else if (is_string(ch, fptr))
        {
            // ungetc(ch, fptr);
            newState = STRING;
        }
        // detect keyword/identifier, assume keyword to start
        else if (isalpha(ch))
        {
            newState = KEYWORD;
        }
        // detect operator
        else if (is_single_op(ch) || has_double_op(ch))
        {
            newState = OPERATOR;
        }
        // TODO: detect character literal
        else if (ch == '\'')
        {
            newState = CHAR_LITERAL;
        }

        // detect numerical literal
        else if (isdigit(ch))
        {
            newState = NUM_LITERAL;
        }
        // detect unknown lexeme
        else
        {
            newState = UNKNOWN;
        }
    }

    // return detected state
    return newState;
}

/**
 * @brief Initialize a given buffer of size size and set all indices to null
 *
 * @param buffer (char *) buffer to (re)initialize
 * @param size (int) size of buffer
 */
void init_buffer(char buffer[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        buffer[i] = '\0';
    }
}

/**
 * @brief Detect if current character is start of comment, return 1 if true,
 * 0 if false.
 *
 * @param ch (char) current character
 * @param fptr (FILE *) file stream to parse
 * @return int 1 if comment, 0 if not.
 */
int is_comment(char ch, FILE *fptr)
{
    char peek[PEEK_BUFFER_LEN];
    if (fpeek(fptr, peek, 1) == EOF)
    {
        return EOF;
    }

    // fpeek(fptr, peek, 1);
    if (ch == '/' && peek[0] == '*')
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Detect if current character is start of string literal. Return 1
 * if true, 0 if false
 *
 * @param ch (char) current character
 * @param fptr (FILE *) file being parsed
 * @return int 1 for true, 0 for false
 */
int is_string(char ch, FILE *fptr)
{
    char peek[PEEK_BUFFER_LEN];
    if (fpeek(fptr, peek, 1) == EOF)
    {
        return EOF;
    }

    if (ch == '"')
    {
        return 1;
    }

    return 0;
}

/**
 * @brief check if current char is an operator that has a compound version.
 * Return 1 if true, 0 if false
 *
 * @param ch (char) current file stream character
 * @param lexeme (char *) currently collected lexeme
 * @return int 1 if true, 0 if false
 */
int is_double_op(char ch, char lexeme[])
{
    static const char *d_operators[] = {
        ":=", "..", "<<", ">>", "<>", "<=", ">=", "**", "!=", "=>"
    };
    size_t size = sizeof(d_operators) / sizeof(char *);

    char temp[3] = {lexeme[0]};
    temp[1] = ch;

    int result = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(temp, d_operators[i]) == 0)
        {
            result = 1;
            break;
        }
    }

    return result;
}

/**
 * @brief Detect if current character is (start of) an operator. Return 1 if
 * true, 0 if false.
 *
 * @param ch (char) current file stream character.
 * @return int 1 if true, 0 if false.
 */
int is_op_symbol(char ch)
{
    int status;
    switch (ch)
    {
        // INTENTIONAL FALLTHROUGH
        case '.':
        case '<':
        case '>':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '|':
        case '&':
        case ';':
        case ',':
        case ':':
        case '[':
        case ']':
        case '=':
        case '!':
            status = 1;
            break;

        default:
            status = 0;
            break;
    }

    return status;
}

/**
 * @brief Detect end of comment. Return 1 for true, 0 for false.
 *
 * @param ch (char) current file stream character.
 * @param fptr (FILE *) Currnt file stream
 * @return int 1 if true, 0 if false.
 */
int is_comment_end(char ch, FILE *fptr)
{
    char peek[1];
    if (fpeek(fptr, peek, 1) == EOF)
    {
        return EOF;
    }

    if (ch == '*' && peek[0] == '/')
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Detect string end. Return 1 for true, 0 for false.
 *
 * @param ch (char) current file stream character.
 * @return int 1 if true, 0 if false.
 */
int is_string_end(char ch)
{
    if (ch == '"')
    {
        return 1;
    }
    return 0;
}

// /**
//  * @brief Detect if current char is whitespace character.
//  *
//  * @param ch (char) currnt file stream character.
//  * @return int 1: true, 0: false
//  */
// int is_whitespace(char ch)
// {
//     if (ch == ' ' || ch == '\n' || ch == '\t')
//     {
//         return 1;
//     }
//     return 0;
// }

/**
 * @brief Detect if current operator is only a single operator.
 *
 * @param ch (char) current operator character
 * @return int 1: true, 0: false.
 */
int is_single_op(char ch)
{
    int status;
    switch (ch)
    {
        // INTENTIONAL FALLTHROUGH
        case '(':
        case ')':
        case '+':
        case '-':
        case '/':
        case '|':
        case '&':
        case ';':
        case ',':
        case '[':
        case ']':
            status = 1;
            break;

        default:
            status = 0;
            break;
    }

    return status;
}

/**
 * @brief Detect if operator character has a compound version.
 *
 * @param ch (char) current operator character
 * @return int 1: true, 0: false
 */
int has_double_op(char ch)
{
    int status;
    switch (ch)
    {
        // INTENTIONAL FALLTHROUGH
        case '.':
        case '<':
        case '>':
        case ':':
        case '=':
        case '*':
        case '!':
            status = 1;
            break;

        default:
            status = 0;
            break;
    }

    return status;
}

/**
 * @brief Detect if current character is allowed identifier character. Return 1
 * for true, 0 for false.
 *
 * @param ch (char) current identifier character.
 * @return int 1: true, 0:false.
 */
int is_allowed_ident_char(char ch)
{
    if (isalnum(ch) || ch == '_')
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Detect if current lexeme is valid keyword. Return 1
 * if true, 0 if false.
 *
 * @param lexeme (char *) current lexeme
 * @return int 1: true, 0: false
 */
int is_keyword(char lexeme[])
{
    // current list of keywords/reserved words
    static const char *keywords[] = {
        "accessor", "and", "array", "begin", "bool", "case", "character",
        "constant", "else", "elsif", "end", "exit", "function", "if", "in",
        "integer", "interface", "is", "loop", "module", "mutator", "natural",
        "null", "of", "or", "others", "out", "positive", "procedure", "range",
        "return", "struct", "subtype", "then", "type", "when", "while"
    };

    // calculate size of keyword list
    size_t size = (sizeof(keywords) / sizeof(char *));

    // look for current lexeme in keywords
    for (size_t i = 0; i < size; ++i)
    {
        // if found, return true
        if (strcmp(lexeme, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0; // if not found, return false.
}

/**
 * @brief Check if character is delimiter for character literal: '
 *
 * @param ch (char) character to compare
 * @return (int) boolean value: 1 if true, 0 if false
 */
int is_char_delim(char ch)
{
    if (ch == '\'')
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Determine if ch is end of numeric literal. Also takes a file pointer
 * of the current file stream being parsed. Returns 1 if true, 0 if false.
 *
 * @param ch (char) character to check for end of literal.
 * @param fptr (FILE *) current file stream pointer.
 * @return (int) 1 if ch is end of numeric literal, 0 if not.
 */
int is_num_literal_end(char ch, FILE *fptr)
{
    char peek[PEEK_BUFFER_LEN];
    fpeek(fptr, peek, 1);

    if ((ch == '.' && peek[0] == '.') || // detected ".." operator
        (ch == ',' && !is_num_lit_char(peek[0])) // detected "," operator.
        )
    {
        return 1;
    }

    if (!is_num_lit_char(ch))
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Check if currrent character is a valid numeric literal character. return 1 if allowed
 * character, 0 if not.
 *
 * @param ch (char) the character being analyzed
 * @return (int) 1 if valid, 0 if not.
 */
int is_num_lit_char(char ch)
{
    if (isxdigit(ch) ||
        ch == '#' ||
        ch == '.' ||
        ch == ',')
    {
        return 1;
    }
    return 0;
}
