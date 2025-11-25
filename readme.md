# Lexical Analyzer Project

## Overview

This project implements a Lexical Analyzer (Lexer) in C, a core component of a compiler. It reads a C source code file and converts it into a sequence of tokens—keywords, identifiers, operators, literals, and delimiters—for use in syntax analysis.

## Features
- Tokenizes C source code into keywords, identifiers, operators, literals, and delimiters
- Detects invalid or unrecognized tokens
- Handles comments and whitespace
- Outputs a stream of tokens with their type and value

## Supported Tokens

| Token Type  | Examples                                   |
|-------------|--------------------------------------------|
| Keyword     | int, return, if, else, while               |
| Identifier  | Variable names, function names             |
| Operator    | +, -, *, /, =, ==, !=, &                   |
| Literal     | Integer, float, character, string literals |
| Delimiter   | (, ), {, }, ;, ,                           |

## Getting Started

### Prerequisites

- GCC or any standard C compiler

### Compilation

gcc lexer.c 

### Usage

Run the lexical analyzer on a C source file: ./lexer input.c

## Sample Input

printf("Enter the second number: ");
scanf("%d", &num2);


## Sample Output

Token: printf      Type: Identifier
Token: (           Type: Delimiter
Token: "Enter the second number: " Type: Literal (String)
Token: )           Type: Delimiter
Token: ;           Type: Delimiter
Token: scanf       Type: Identifier
Token: (           Type: Delimiter
Token: "%d"        Type: Literal (String)
Token: ,           Type: Delimiter
Token: &           Type: Operator
Token: num2        Type: Identifier
Token: )           Type: Delimiter
Token: ;           Type: Delimiter


## Author

Developed by [Varshini]. Contact: [varshiniyadav87@gmail.com]

