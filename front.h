/*
=============================================================================
Title       : front.h
Description : Token definitions and function prototypes for the analyzer.
Author      : James Carrington Jr. 
Date        : 04/02/2026
Version     : 1.0
Usage       : Included in front.c
Notes       : Sectioned for readability by grouping literals and operators.
C Version   : C99 / C11
=============================================================================
*/

#ifndef FRONT_H
#define FRONT_H

#include <stdio.h>

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token code list */

/* Identifiers 10's */
#define INT_LITERAL    10  /* I */
#define IDENT          11  /* V */
#define FLOAT_LITERAL  12  /* L */

/* Basic operations 20's */
#define ASSIGNMENT_OP  20  /* := */
#define ADD_OP         21  /* +  */
#define SUB_OP         22  /* -  */
#define MULT_OP        23  /* * */
#define DIV_OP         24  /* /  */
#define MOD_OP         25  /* %  */
#define INC_OP         26  /* ++ */
#define DEC_OP         27  /* -- */


/* Comparison operator symbols 30's */
#define LESSER_OP      30  /* <  */
#define GREATER_OP     31  /* >  */
#define EQUAL_OP       32  /* == */
#define NEQUAL_OP      33  /* <> */
#define LEQUAL_OP      34  /* <= */
#define GEQUAL_OP      35  /* >= */
#define BOOLEAN_OR     36  /* or */
#define BOOLEAN_AND    37  /* and*/
#define BOOLEAN_NOT    38  /* not*/

/* Punctuation symbols 40's */
#define SEMICOLON      40  /* ;  */
#define COLON          41  /* :  */
#define LEFT_PAREN     42  /* (  */
#define RIGHT_PAREN    43  /* )  */
#define SHIFT_R_OP     44  /* >> */
#define SHIFT_L_OP     45  /* << */
#define RANGE_OP       46  /* .. */

/* Keywords Operators 50's */
#define KEY_IF         50
#define KEY_ELIF       51
#define KEY_ELSE       52
#define KEY_FOR        53
#define KEY_IN         54
#define KEY_CIN        55
#define KEY_COUT       56

#define INT_LIT INT_LITERAL
#define FLOAT_LIT FLOAT_LITERAL
#define ASSIGN_OP ASSIGNMENT_OP

int lex();
char getChar();

extern int nextToken;
extern int lineCount;
extern char lexeme[100];
extern FILE *in_fp;



#endif
