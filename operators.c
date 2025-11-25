#include "lexer.h"

int isOperator(char ch){
    // Check if a single character is in the list of operators
    return strchr(op1, ch) != NULL;
}

int isDualOperator(char buffer[]){
    // Check if the string buffer matches any dual-character operator
    // NOTE: You are iterating over 16 elements here; make sure op2[] has 16 valid entries
    for(int i = 0 ; i < 16; i++)
        if(strcmp(op2[i], buffer) == 0)
            return 1;
    return 0;
}

void handleOperators(FILE *fp, char buffer[], char ch){
    int index = 0;
    buffer[index++] = ch;  // Store first operator character
    currentCol++;           // Track first character

    char nextChar = fgetc(fp);  // Look ahead for possible dual operator

    if(nextChar == EOF){
        // Single operator at end of file
        printf("Operators : %c\n", ch);
        memset(buffer, 0, index);
        return;
    }

    buffer[index++] = nextChar;
    buffer[index] = '\0';
    currentCol++;           // Track second character

    if(isDualOperator(buffer)){
        // Valid dual-character operator found
        printf("Operators : %s\n", buffer);
    }
    else{
        // Only first character is an operator
        printf("Operators : %c\n", ch);  
        ungetc(nextChar, fp);  // Push back unused character
        currentCol--;          // Adjust column count
    }
    
    memset(buffer, 0, index);  // Clear buffer
}
