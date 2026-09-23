/*
=============================================================================
Title       : parser.h
Description : Function prototypes and external variables for the Cooke Parser.
Author      : James Carrington Jr. 
Date        : 04/02/2026
Version     : 1.0
Usage       : Included in parser.c and front.c
Notes       : Defines the recursive descent structure for Cooke BNF rules.
C Version   : C99 / C11
=============================================================================
*/

#ifndef PARSER_H
#define PARSER_H

/* Root and Statements */
void parse_P(); 
void parse_S(); 
void parse_M(); 

/* Boolean and Comparisons */
void parse_C(); 
void parse_A(); 
void parse_R(); 

/* Arithmetic Expressions */
void expr();    
void term();    
void factor();  

void match(int expectedToken);
const char* get_token_name(int token);

/* External variables from front.c */
extern int nextToken;
extern int lineCount;
extern char lexeme[100];
extern FILE *in_fp;

#endif
