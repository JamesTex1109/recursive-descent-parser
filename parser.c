/*
=============================================================================
Title       : parser.c
Description : Recursive descent syntax parser for the Cooke language.
Author      : James Carrington Jr. 
Date        : 04/02/2026
Version     : 1.0
Usage       : Compile with 'make' and run ./cooke_parser [input_file]
Notes       : Validates syntax based on the provided BNF grammar rules.
C Version   : C99 / C11
=============================================================================
*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "parser.h"
#include "front.h"

// SECTION 1: HELPERS & TOKEN NAMES

int is_statement_starter(int token) {
    return (token == IDENT || token == KEY_IF || token == KEY_FOR || 
            token == KEY_CIN || token == KEY_COUT || token == INC_OP || 
            token == DEC_OP || token == INT_LITERAL || token == FLOAT_LITERAL);
}

const char* get_token_name(int token) {
    switch (token) {
        case IDENT: return "IDENT";
        case INT_LITERAL: return "INT_LITERAL";
        case FLOAT_LITERAL: return "FLOAT_LITERAL";
        case ASSIGNMENT_OP: return "ASSIGNMENT_OP";
        case ADD_OP: return "ADD_OP";
        case SUB_OP: return "SUB_OP";
        case MULT_OP: return "MULT_OP";
        case DIV_OP: return "DIV_OP";
        case MOD_OP: return "MOD_OP";
        case INC_OP: return "INC_OP";
        case DEC_OP: return "DEC_OP";
        case LESSER_OP: return "LESSER_OP";
        case GREATER_OP: return "GREATER_OP";
        case EQUAL_OP: return "EQUAL_OP";
        case NEQUAL_OP: return "NEQUAL_OP";
        case LEQUAL_OP: return "LEQUAL_OP";
        case GEQUAL_OP: return "GEQUAL_OP";
        case BOOLEAN_OR: return "BOOLEAN_OR";
        case BOOLEAN_AND: return "BOOLEAN_AND";
        case BOOLEAN_NOT: return "BOOLEAN_NOT";
        case SEMICOLON: return "SEMICOLON";
        case COLON: return "COLON";
        case LEFT_PAREN: return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case SHIFT_R_OP: return "SHIFT_R_OP";
        case SHIFT_L_OP: return "SHIFT_L_OP";
        case RANGE_OP: return "RANGE_OP";
        case KEY_IF: return "KEY_IF";
        case KEY_ELIF: return "KEY_ELIF";
        case KEY_ELSE: return "KEY_ELSE";
        case KEY_FOR: return "KEY_FOR";
        case KEY_IN: return "KEY_IN";
        case KEY_CIN: return "KEY_CIN";
        case KEY_COUT: return "KEY_COUT";
        case -1: return "EOF";
        default: return "UNKNOWN";
    }
}

//* SECTION 2: MAIN & MATCH

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No input file provided.\n");
        exit(2);
    }

    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("Error: File %s not found.\n", argv[1]);
        exit(3);
    }

    printf("Cooke Parser\n");

    getChar(); 
    lex();    

    parse_P();

    return 0;
}

void match(int expected) {
    if (nextToken == expected) {
        lex();
    } else {
        printf("Error encounter on line %d: The next lexeme was %s and the next token was %s\n", 
               lineCount, lexeme, get_token_name(nextToken));
        exit(1);
    }
}

//SECTION 3: TOP-LEVEL PROGRAM (parse_P)

void parse_P() {
    while (nextToken != -1) {
        if (is_statement_starter(nextToken)) {
            parse_S();
            // Top-level separator
            if (nextToken == SEMICOLON) {
                lex();
                // If file ends right after semicolon, it's a trailing error
                if (nextToken == -1) match(IDENT); 
            }
        } else if (nextToken == SEMICOLON) {
            lex(); // stray semicolon
            if (nextToken == -1) match(IDENT);
        } else {
            break;
        }
    }
    printf("Syntax Validated\n");
    exit(0);
}

//SECTION 4: STATEMENTS (parse_S)

void parse_S() {
    switch (nextToken) {
        case IDENT:
            match(IDENT);
            match(ASSIGNMENT_OP);
            expr();
            break;

        case KEY_CIN:
            match(KEY_CIN);
            match(SHIFT_R_OP);
            match(IDENT);
            break;

        case KEY_COUT:
            match(KEY_COUT);
            match(SHIFT_L_OP);
            expr();
            break;

        case KEY_IF:
            match(KEY_IF);
            parse_C();
            match(COLON);
            while (nextToken != -1 && nextToken != KEY_ELIF && nextToken != KEY_ELSE) {
                if (is_statement_starter(nextToken)) {
                    parse_S();
                    // SEPARATOR LOGIC:
                    // 1. If another statement follows, we MUST have a semicolon.
                    if (is_statement_starter(nextToken)) {
                        match(SEMICOLON);
                    } 
                    // 2. If we see a semicolon but the block is ending, that is an error.
                    else if (nextToken == SEMICOLON) {
                        lex();
                        if (nextToken == KEY_ELIF || nextToken == KEY_ELSE || nextToken == -1) {
                            match(IDENT); // Force error for trailing semicolon
                        }
                    }
                } else break;
            }
            parse_M(); 
            break;

        case KEY_FOR:
            match(KEY_FOR);
            match(IDENT);
            match(KEY_IN);
            expr();
            match(RANGE_OP);
            expr();
            if (nextToken == COLON) match(COLON);
            
            while (nextToken != -1 && nextToken != KEY_ELIF && nextToken != KEY_ELSE) {
                if (is_statement_starter(nextToken)) {
                    parse_S();
                    if (is_statement_starter(nextToken)) {
                        match(SEMICOLON);
                    } else if (nextToken == SEMICOLON) {
                        lex();
                        if (nextToken == KEY_ELIF || nextToken == KEY_ELSE || nextToken == -1) {
                            match(IDENT);
                        }
                    }
                } else break;
            }
            break;

        case INC_OP:
        case DEC_OP:
            match(nextToken);
            match(IDENT);
            break;

        case INT_LITERAL:
        case FLOAT_LITERAL:
            lex(); 
            break;

        case SEMICOLON:
            lex(); 
            break;

        default:
            if (nextToken != -1) match(IDENT); 
            break;
    }
}

//SECTION 5: IF-ELIF-ELSE LOGIC (parse_M)

void parse_M() {
    if (nextToken == KEY_ELIF) {
        match(KEY_ELIF);
        parse_C();
        match(COLON);
        
        while (nextToken != -1 && nextToken != KEY_ELIF && nextToken != KEY_ELSE) {
            if (is_statement_starter(nextToken)) {
                parse_S();
                if (is_statement_starter(nextToken)) {
                    match(SEMICOLON);
                } else if (nextToken == SEMICOLON) {
                    lex();
                    if (nextToken == KEY_ELIF || nextToken == KEY_ELSE || nextToken == -1) {
                        match(IDENT);
                    }
                }
            } else break;
        }
        parse_M();
    } 
    else if (nextToken == KEY_ELSE) {
        match(KEY_ELSE);
        match(COLON);
        
        while (nextToken != -1) {
            if (is_statement_starter(nextToken)) {
                parse_S();
                if (is_statement_starter(nextToken)) {
                    match(SEMICOLON);
                } else if (nextToken == SEMICOLON) {
                    lex();
                    if (nextToken == -1) { // End of file check for ELSE
                        match(IDENT);
                    }
                }
            } else break;
        }
    }
}

//SECTION 6: CONDITIONS & BOOLEANS (parse_C, parse_A, parse_R)

void parse_C() {
    parse_A(); 
    while (nextToken == BOOLEAN_OR) {
        match(BOOLEAN_OR);
        parse_A();
    }
}

void parse_A() {
    parse_R(); 
    while (nextToken == BOOLEAN_AND) {
        match(BOOLEAN_AND);
        parse_R();
    }
}

void parse_R() {
    if (nextToken == BOOLEAN_NOT) {
        match(BOOLEAN_NOT);
        parse_R();
    } else {
        expr(); 
        int ops[] = {LESSER_OP, GREATER_OP, EQUAL_OP, NEQUAL_OP, LEQUAL_OP, GEQUAL_OP};
        int is_relop = 0;
        for(int i=0; i<6; i++) if(nextToken == ops[i]) is_relop = 1;

        if (is_relop) {
            match(nextToken);
            expr(); 
        }
    }
}

//SECTION 7: MATH EXPRESSIONS (expr, term, factor

void expr() {
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        match(nextToken); 
        term();
    }
}

void term() {
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
        match(nextToken); 
        factor();
    }
}

void factor() {
    if (nextToken == IDENT) {
        match(IDENT);
    } else if (nextToken == INC_OP) { 
        match(INC_OP);
        match(IDENT);
    } else if (nextToken == DEC_OP) { 
        match(DEC_OP);
        match(IDENT);
    } else if (nextToken == INT_LITERAL || nextToken == FLOAT_LITERAL) {
        lex();
    } else if (nextToken == LEFT_PAREN) {
        match(LEFT_PAREN);
        expr();
        match(RIGHT_PAREN);
    } else {
        match(IDENT); 
    }
}