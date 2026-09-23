/*
=============================================================================
Title       : front.c
Description : Lexical analyzer implementation for the Cooke language.
Author      : James Carrington Jr. 
Date        : 04/02/2026
Version     : 1.0
Usage       : Compile with 'make' and run ./cooke_analyzer [input_file]
Notes       : Strictly follows the Cooke language token specification.
C Version   : C99 / C11
=============================================================================
*/

#include <stdio.h>

#include <ctype.h>

#include <string.h>

#include "front.h"


/* Global Variable */

  int nextToken;
  int lineCount = 1; 

/* Local Variables */

  static int charClass;

  char lexeme [100];

  static char nextChar;

  static int lexLen;

  FILE *in_fp;

/* Local Function declarations */

void addChar();
char getChar();
int lookup(char ch);

/*New Functions*/

int isKeyword(char *text);

/* addChar - a function to add nextChar to lexeme */

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/

/* getChar - gets the next character and sets class */

char getChar() {
    if ((nextChar = getc(in_fp)) != EOF) { //Incremented Line count command

        if (isalpha(nextChar) || nextChar == '_') 
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else 
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
    return nextChar;
}

/* Token functions lookup section */

 int lookup(char ch) {
    char next;
    switch (ch) {
        case '+':
            addChar();
            next = getChar();
            if (next == '+') {
                addChar();
                nextToken = INC_OP;      // 26
            } else {
                ungetc(next, in_fp);
                nextToken = ADD_OP;      // 21
            }
            break;
        case '-':
            addChar();
            next = getChar();
            if (next == '-') {
                addChar();
                nextToken = DEC_OP;      // 27
            } else {
                ungetc(next, in_fp);
                nextToken = SUB_OP;      // 22
            }
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;         // 23
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;          // 24
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;          // 25
            break;
        case ':':
            addChar();
            next = getChar();
            if (next == '=') {
                addChar();
                nextToken = ASSIGNMENT_OP; // 20
            } else {
                ungetc(next, in_fp);
                nextToken = COLON;         // 41
            }
            break;
            
            case '=':
            addChar();
            next = getChar();
            if (next == '=') {
                addChar();
                nextToken = EQUAL_OP;    // 32
            } else {
                ungetc(next, in_fp);
                nextToken = UNKNOWN;     // 
            }
            break;
        case '<':
            addChar();
            next = getChar();
            if (next == '>') {
                addChar();
                nextToken = NEQUAL_OP;   // 33
            } else if (next == '=') {
                addChar();
                nextToken = LEQUAL_OP;   // 34
            } else if (next == '<') {
                addChar();
                nextToken = SHIFT_L_OP;  // 45
            } else {
                ungetc(next, in_fp);
                nextToken = LESSER_OP;   // 30
            }
            break;
        case '>':
            addChar();
            next = getChar();
            if (next == '=') {
                addChar();
                nextToken = GEQUAL_OP;   // 35
            } else if (next == '>') {
                addChar();
                nextToken = SHIFT_R_OP;  // 44
            } else {
                ungetc(next, in_fp);
                nextToken = GREATER_OP;  // 31
            }
            break;

            case ';':
            addChar();
            nextToken = SEMICOLON;       // 40
            break;
        case '(':
            addChar();
            nextToken = LEFT_PAREN;      // 42
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;     // 43
            break;
        
        default:
            addChar();
            nextToken = UNKNOWN;         // 99
            break;
    }
    return nextToken;
}

/*****************************************************/

/* getNonBlank */
/* character */

static void getNonBlank() {
    while (isspace(nextChar)) {
        if (nextChar == '\n') {
            lineCount++;
        }
        getChar(); // Advance to the next character
    }
}

/* lex section */

int lex() {
    lexLen = 0;
    lexeme[0] = '\0'; 
    
    // 1. Skip all whitespace first
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers and Keywords */
        case LETTER:
            addChar();
            getChar();
            // ADDED: nextChar == '_' to allow underscores in names
            while (charClass == LETTER || charClass == DIGIT || nextChar == '_') {
                addChar();
                getChar();
            }
            nextToken = IDENT; 
            if (isKeyword(lexeme) != 0)
                nextToken = isKeyword(lexeme);
            break;

        /* Parse integer and float literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LITERAL;

            /* PEEK LOGIC: Only treat '.' as a float if the NEXT char is also a digit */
            /* This prevents it from eating the first half of '..' */
            if (nextChar == '.') {
                // We need to look ahead one more character without consuming it
                // If your getChar system doesn't support ungetc, we check manually:
                int c = getc(in_fp);
                if (isdigit(c)) {
                    ungetc(c, in_fp); 
                    addChar();        
                    getChar();        
                    while (charClass == DIGIT) {
                        addChar();
                        getChar();
                    }
                    nextToken = FLOAT_LITERAL;
                } else {
                    ungetc(c, in_fp); 
                }
            }
            break;

        /* Parentheses, operators, and Range Op */
       case UNKNOWN:
            if (nextChar == '.') {
                addChar();
                getChar();
                if (nextChar == '.') {
                    addChar();
                    getChar();
                    nextToken = RANGE_OP;
                } else {
                    // Instead of calling lookup, we identify it here
                    nextToken = FLOAT_LITERAL; 
                }
            } else {
                lookup(nextChar);
                getChar();
            }
            break;

        case EOF:
            nextToken = -1;
            lexeme[0] = '\0';
            break;
    }

    
    return nextToken;
}

// lookup function for keywords
// matches Cooke language keywords to your front.h codes

int isKeyword(char *text) {
    if (strcmp(text, "if") == 0)
        return 50; // KEY_IF
    else if (strcmp(text, "elif") == 0)
        return 51; // KEY_ELIF
    else if (strcmp(text, "else") == 0)
        return 52; // KEY_ELSE
    else if (strcmp(text, "for") == 0)
        return 53; // KEY_FOR
    else if (strcmp(text, "in") == 0)
        return 54; // KEY_IN
    else if (strcmp(text, "cin") == 0)
        return 55; // KEY_CIN
    else if (strcmp(text, "cout") == 0)
        return 56; // KEY_COUT
    else if (strcmp(text, "and") == 0)
        return 37; // BOOLEAN_AND
    else if (strcmp(text, "or") == 0)
        return 36; // BOOLEAN_OR
    else if (strcmp(text, "not") == 0)
        return 38; // BOOLEAN_NOT
    else
        return 0;  // Not a keyword
}
