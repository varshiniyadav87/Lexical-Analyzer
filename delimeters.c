#include "lexer.h"

// Checks if a character is a delimiter (like (, ), {, }, etc.)
int isDelimeter(char ch){
    // strchr returns a pointer to the first occurrence of ch in delimiters, or NULL if not found
    return strchr(delimiters, ch) != NULL;
}
// Handles delimiter characters: updates counters and reports unmatched closing delimiters
void handleDelimiter(char ch)
{
    switch(ch) {
        case '(': leftParen++; break;
        case ')':
            rightParen++;
            // If there are more closing parens than opening, report an unmatched ')'
            if (rightParen > leftParen)
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Unmatched ')'\n", currentLine, currentCol);
            break;

        case '{': leftBrace++; break;
        case '}':
            rightBrace++;
            if (rightBrace > leftBrace)
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Unmatched '}'\n", currentLine, currentCol);
            break;

        case '[': leftBracket++; break;
        case ']':
            rightBracket++;
            if (rightBracket > leftBracket)
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Unmatched ']'\n", currentLine, currentCol);
            break;
    }
    printf("Delimiter : %c\n", ch);
    fflush(stdout);
}

// After processing the file, checks if any opening delimiters don't have matching closings
void checkUnmatchedDelimiters() 
{
    if (leftParen > rightParen)
        fprintf(stderr,"Lexical Error: %d unmatched '(' remain\n", leftParen - rightParen);
    if (leftBrace > rightBrace)
        fprintf(stderr,"Lexical Error: %d unmatched '{' remain\n", leftBrace - rightBrace);
    if (leftBracket > rightBracket)
        fprintf(stderr,"Lexical Error: %d unmatched '[' remain\n", leftBracket - rightBracket);
}
