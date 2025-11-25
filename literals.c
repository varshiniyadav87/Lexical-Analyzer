#include "lexer.h"

void validStringLiteral(FILE *fp , char buffer[], char ch) {
    int index = 0;
    int terminated = 0; // flag to check if string closed properly

    buffer[index++] = ch;  // Store opening quote
    currentCol++;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            // Unterminated string literal at newline -> report error
            fprintf(stderr, "Lexical Error at line %d, column %d: Unterminated string literal\n", currentLine, currentCol);
            currentLine++;
            currentCol = 1;

            // Skip remaining characters until next quote or newline
            while (ch != EOF && ch != '\"' && ch != '\n') {
                ch = fgetc(fp);
                currentCol++;
            }
            break;
        }

        if (index >= MAX_LEN - 1) {  // Prevent buffer overflow
            fprintf(stderr, "Lexical Error: String literal too long at line %d, column %d\n", currentLine, currentCol);

            // Skip remaining characters until closing quote or newline
            while (ch != EOF && ch != '\"' && ch != '\n') {
                ch = fgetc(fp);
                currentCol++;
            }
            break;
        }

        buffer[index++] = ch;

        if (ch == '\"') { // closing quote found
            terminated = 1;
            currentCol++;
            break;
        }

        currentCol++;
    }

    buffer[index] = '\0';

    if (terminated) {
        printf("String Literal : %s\n", buffer);
    }

    memset(buffer, 0, index);  // Clear buffer
}


void validCharLiteral(FILE *fp,char buffer[],char ch){
    int index = 0;
    
    buffer[index++] = ch;     // Opening '
    currentCol++;

    char c1 = fgetc(fp);      // The actual character
    if(c1 == EOF){
        fprintf(stderr, "Lexical Error at line %d, column %d: Unterminated character literal at EOF\n", currentLine, currentCol);
        return;
    }
    currentCol++;

    if(c1 == '\'') {
        // Empty character literal ''
        fprintf(stderr, "Lexical Error at line %d, column %d: Empty character literal\n", currentLine, currentCol);
        return;
    }

    if(c1 == '\n') {
        fprintf(stderr, "Lexical Error at line %d, column %d: Unterminated character literal\n", currentLine, currentCol);
        currentLine++;
        currentCol = 1;
        return;
    }

    buffer[index++] = c1;     // Store character

    char c2 = fgetc(fp);      // Expect closing '
    if(c2 == EOF) {
        fprintf(stderr, "Lexical Error at line %d, column %d: Unterminated character literal at EOF\n", currentLine, currentCol);
        return;
    }
    currentCol++;

    if(c2 != '\'') {           // Closing quote missing
        fprintf(stderr, "Lexical Error at line %d, column %d: Character literal must be closed with '\n", currentLine, currentCol);

        // Skip to next delimiter or newline to recover
        while(c2 != EOF && c2 != '\'' && c2 != ';' && c2 != '\n') {
            c2 = fgetc(fp);
            currentCol++;
        }

        if(c2 == '\n') {
            currentLine++;
            currentCol = 1;
        }
        return;
    }

    buffer[index++] = c2;     // Closing '
    buffer[index] = '\0';

    printf("Character Literal : %s\n", buffer);
    memset(buffer, 0, index);
}
