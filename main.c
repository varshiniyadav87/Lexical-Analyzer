/* 
    File Name       : main.c
    Author          : Adi Kesava Reddy
    Date            : 23/11/2025

    Description     : 
    This project implements a Lexical Analyzer for C-like source code.
    It supports:
        • Identifying Keywords, Identifiers, and Operators
        • Parsing Delimiters including (), {}, []
        • Handling String and Character Literals
        • Recognizing numeric literals: decimal, hexadecimal, octal, binary
        • Reporting lexical errors with line and column information

    The program uses helper functions to:
        • Validate tokens
        • Count and check balanced delimiters
        • Detect invalid characters and malformed literals
        • Print detailed output for each token type
*/ 

#include "lexer.h"      // Custom header presumably containing function declarations and constants

// Global variables for tracking line and column numbers in the source file
int currentLine = 1;
int currentCol = 1;

// Counters for keeping track of matching pairs of delimiters
int leftParen = 0, rightParen = 0;
int leftBrace = 0, rightBrace = 0;
int leftBracket = 0, rightBracket = 0;

// Array of C keywords used for recognizing identifiers
char *keywords[] = {
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof",
    "static","struct","switch","typedef","union","unsigned",
    "void","volatile","while"
};

// Array of single character delimiters used in C
char delimiters[] = {'(', ')', '{', '}', '[', ']', ';', ',' , '.', ':'};

// Array of single-character operators
char op1[] = {'+', '-', '*', '/', '%', '=', '<', '>', '&', '|', '^', '~', '!', '?', ':', '.', ','};

// Array of multi-character operators (strings)
char *op2[] = {"==", "!=", "<=", ">=", "++", "--", "&&", "||", "<<", ">>", "->","+=", "-=", "*=", "/=", "%="};

int main() {
    FILE *fp;
    // Open the file "file.txt" in read mode
    fp = fopen("file.txt","r");
    
    // If file can't be opened, exit program
    if(fp == NULL) return 0;

    char ch;
    char buffer[MAX_LEN];  // Buffer to store tokens
    int index = 0;         // Used to track buffer index, though not used explicitly here

    // Read characters till end of file
    while((ch = fgetc(fp)) != EOF) {
        // Handle newlines and carriage return: increment line count and reset column
        if (ch == '\n' || ch =='\r' || ch == '#') {
            currentLine++;
            currentCol = 1;
            continue;  // Skip further processing of this character
        }
        // Handle spaces and tabs: increment the column count and continue
        if(ch == ' ' || ch == '\t') {
            currentCol++;
            continue;
        }
        // If character is alphabetic or underscore, process keywords or identifiers
        if (isalpha(ch) || ch == '_'){
            handleKeywords(fp, buffer, ch);
        }
        // If character is an operator, process operators
        else if(isOperator(ch)){
            handleOperators(fp, buffer, ch);
        }
        // If character is double quote, process string literal
        else if(ch == '\"') {
            validStringLiteral(fp, buffer, ch);
        }
        // If character is single quote, process char literal
        else if(ch == '\'') {
            validCharLiteral(fp, buffer, ch);
        }
        // If character is a delimiter, handle it accordingly
        else if(isDelimeter(ch)) {
            handleDelimiter(ch);
        }
        // If character is digit, process numbers
        else if(isdigit(ch)){
            handleDigit(fp, buffer, ch);
        }
        else {
            // For any other character not matching the above, report lexical error
            fprintf(stderr, "Lexical Error at line %d, column %d: Invalid Character %d (ASCII)\n", currentLine, currentCol, ch);
            currentCol++;
        }
    }

    // Close the file after processing all characters
    fclose(fp);

    // Check for any unmatched delimiters after complete processing
    checkUnmatchedDelimiters();

    return 0;
}
