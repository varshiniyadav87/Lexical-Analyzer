#include "lexer.h"

extern char *keywords[];  // The list of all keywords declared elsewhere (probably lexer.c)

// Function to check if a given string matches a keyword
int isKeyword(char buffer[]){
    for(int i = 0; i < 32; i++)           // Loop through all keywords
        if(strcmp(keywords[i], buffer) == 0)  // Compare strings
            return 1;                     // Match found → it is a keyword
    return 0;                             // No match → not a keyword
}


// Function to handle identifiers and keywords
void handleKeywords(FILE *fp, char *buffer, char ch) {
    int index = 0;

    // Store the first character (already known to be alphabet or underscore)
    buffer[index++] = ch;

    // Read remaining characters of the identifier
    // Valid identifier chars: alphanumeric or underscore
    while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {

        if (index >= MAX_LEN - 1) {   // Prevent buffer overflow
            fprintf(stderr, "Lexical Error at line %d, column %d: Identifier too long\n",
                    currentLine, currentCol);

            // Consume remaining characters of the identifier to avoid corrupting next token
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {}

            break;  // Stop adding characters to buffer
        }

        buffer[index++] = ch;   // Add character to buffer
    }

    buffer[index] = '\0';  // Null-terminate the string safely

    // Check if the string is a keyword
    if (isKeyword(buffer)) {
        printf("Keyword : %s\n", buffer);   // Print keyword
    }
    else {
        // If identifier starts with a digit, it’s invalid
        if (isdigit(buffer[0])) {
            fprintf(stderr, "Lexical Error at line %d, column %d: Identifier cannot start with a digit\n",
                    currentLine, currentCol);
        }
        else {
            printf("Identifier : %s\n", buffer);  // Print valid identifier
        }
    }

    // Put back the non-identifier character we read but didn't use
    if (ch != EOF)
        ungetc(ch, fp);

    // Clear buffer for next use
    memset(buffer, 0, index);
}
