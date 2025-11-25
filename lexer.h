#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Maximum length for tokens (identifiers, numbers, strings, etc.)
#define MAX_LEN 1000

// Arrays storing delimiters, single-char operators, and dual-char operators
extern char delimiters[];  // e.g., '(', ')', '{', '}', '[', ']', ';', ','
extern char op1[];         // single-character operators: '+', '-', '*', '/', etc.
extern char *op2[];        // dual-character operators: "==", "!=", "<=", ">=", etc.

// Global counters for current position and delimiter tracking
extern int currentLine, currentCol;                   // Current line and column for error reporting
extern int leftParen, rightParen;                    // '(' and ')' counters
extern int leftBrace, rightBrace;                    // '{' and '}' counters
extern int leftBracket, rightBracket;               // '[' and ']' counters

// ---------- Keyword & Operator Checks ----------
int isKeyword(char buffer[]);                        // Check if a string is a keyword
int isDelimeter(char ch);                            // Check if a character is a delimiter
int isOperator(char ch);                             // Check if a character is an operator
int isDualOperator(char buffer[]);                  // Check if a string is a dual-character operator

// ---------- Number Checks ----------
int isoctdigit(char c);                              // Check if character is a valid octal digit ('0'-'7')
int isbindigit(char c);                              // Check if character is binary ('0' or '1')

// ---------- Delimiter Handling ----------
void handleDelimiter(char ch);                       // Process a delimiter and update counters
void checkUnmatchedDelimiters();                     // Print errors for unmatched '(', '{', '['

// ---------- Token Handlers ----------
void handleKeywords(FILE *fp, char *buffer, char ch);      // Handle keywords & identifiers
void handleOperators(FILE *fp, char *buffer, char ch);     // Handle operators
void validStringLiteral(FILE *fp, char buffer[], char ch); // Handle string literals
void validCharLiteral(FILE *fp, char *buffer, char ch);    // Handle character literals
void handleDigit(FILE *fp, char *buffer, char ch);         // Handle numbers (decimal, hex, octal, binary)

// ---------- Number Parsers ----------
void handleHex(FILE *fp, char *buffer, char ch);       // Handle hexadecimal literals
void handleOct(FILE *fp, char *buffer, char ch);       // Handle octal literals
void handleDecimal(FILE *fp, char *buffer, char ch);   // Handle decimal/floating literals

#endif
