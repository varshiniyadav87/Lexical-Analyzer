#include "lexer.h"

// Returns 1 if c is an octal digit (0-7), otherwise 0
int isoctdigit(char c) {
    return (c >= '0' && c <= '7');
}

// Returns 1 if c is a binary digit (0 or 1), otherwise 0
int isbindigit(char c) {
    return (c == '0' || c == '1');
}

// Handles hexadecimal literals (e.g., 0x2F, 0XA3) from the source file
void handleHex(FILE *fp, char *buffer, char prefix) {
    int index = 0;
    buffer[index++] = '0';      // First character ('0')
    buffer[index++] = prefix;   // Prefix ('x' or 'X')
    currentCol++;

    int validDigitSeen = 0;
    char ch;

    // Read hex digits and form the token
    while ((ch = fgetc(fp)) != EOF) {
        if (isxdigit(ch)) {     // Accept: 0-9, a-f, A-F
            buffer[index++] = ch;
            currentCol++;
            validDigitSeen = 1;
        } else break;           // Any non-hex digit stops the loop

        if (index >= MAX_LEN - 1) {  // Overflow check
            fprintf(stderr, "Lexical Error: Hex literal too long\n");
            break;
        }
    }

    // Error if no valid digit is seen after the prefix
    if (!validDigitSeen) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Hex literal missing digits\n",
                currentLine, currentCol);
    }

    buffer[index] = '\0';

    // Handle invalid character within the hex literal
    if (!isxdigit(ch) && ch != EOF) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Invalid hex digit '%c'\n",
                currentLine, currentCol, ch);

        // Skip until a delimiter or whitespace
        while (ch != EOF && !isspace(ch) &&
               ch != ';' && ch != ',' && ch != ')' && ch != '(' &&
               ch != '{' && ch != '}') {
            ch = fgetc(fp);
            currentCol++;
        }

        if (ch != EOF) ungetc(ch, fp);
        return;
    }

    // Push non-token character back for future parsing
    if (ch != EOF) ungetc(ch, fp);

    printf("Hex Number: %s\n", buffer);  // Success: print the hex number
    memset(buffer, 0, index);            // Clear the buffer for reuse
}

// Handles octal literals (e.g., 0o725 or 0O15) from the source file
void handleOctal(FILE *fp, char *buffer, char prefix) {
    int index = 0;
    buffer[index++] = '0';
    buffer[index++] = prefix;   // 'o' or 'O'
    currentCol++;

    int validDigitSeen = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch >= '0' && ch <= '7') {
            buffer[index++] = ch;
            currentCol++;
            validDigitSeen = 1;
        } else break;

        if (index >= MAX_LEN - 1) {
            fprintf(stderr, "Lexical Error: Octal literal too long\n");
            break;
        }
    }

    if (!validDigitSeen) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Octal literal missing digits\n",
                currentLine, currentCol);
    }

    buffer[index] = '\0';

    if (ch != EOF && !(ch >= '0' && ch <= '7')) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Invalid octal digit '%c'\n",
                currentLine, currentCol, ch);

        while (ch != EOF && !isspace(ch) &&
               ch != ';' && ch != ',' && ch != ')' && ch != '(' &&
               ch != '{' && ch != '}') {
            ch = fgetc(fp);
            currentCol++;
        }

        if (ch != EOF) ungetc(ch, fp);
        return;
    }

    if (ch != EOF) ungetc(ch, fp);

    printf("Octal Number: %s\n", buffer);
    memset(buffer, 0, index);
}

// Handles binary literals (e.g., 0b101010 or 0B01001)
void handleBinary(FILE *fp, char *buffer, char prefix) {
    int index = 0;
    buffer[index++] = '0';
    buffer[index++] = prefix;   // 'b' or 'B'
    currentCol++;

    int validDigitSeen = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '0' || ch == '1') {
            buffer[index++] = ch;
            currentCol++;
            validDigitSeen = 1;
        } else break;

        if (index >= MAX_LEN - 1) {
            fprintf(stderr, "Lexical Error: Binary literal too long\n");
            break;
        }
    }

    if (!validDigitSeen) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Binary literal missing digits\n",
                currentLine, currentCol);
    }

    buffer[index] = '\0';

    if (ch != EOF && !(ch == '0' || ch == '1')) {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Invalid binary digit '%c'\n",
                currentLine, currentCol, ch);

        while (ch != EOF && !isspace(ch) &&
               ch != ';' && ch != ',' && ch != ')' && ch != '(' &&
               ch != '{' && ch != '}') {
            ch = fgetc(fp);
            currentCol++;
        }

        if (ch != EOF) ungetc(ch, fp);
        return;
    }

    if (ch != EOF) ungetc(ch, fp);

    printf("Binary Number: %s\n", buffer);
    memset(buffer, 0, index);
}

// Handles decimal/floating-point numbers (e.g., 42, 9.15, 7e-2)  
void handleDecimal(FILE *fp, char *buffer, char ch) {
    int index = 0;
    int seenDot = 0;
    int seenExponent = 0;

    buffer[index++] = ch;
    currentCol++;

    while ((ch = fgetc(fp)) != EOF ) {
        if (isdigit(ch)) {                  // Accept: 0-9
            buffer[index++] = ch;
            currentCol++;
        }
        else if (ch == '.') {               // Only one dot permitted and only before exponent
            if (seenDot || seenExponent) {
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Invalid '.' position\n",
                        currentLine, currentCol);
                break;
            }
            seenDot = 1;
            buffer[index++] = ch;
            currentCol++;
        }
        else if(ch == 'e' || ch == 'E') {   // Start of exponent part
            if (seenExponent){
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Multiple exponent markers\n",
                        currentLine, currentCol);
                break;
            }
            seenExponent = 1;
            buffer[index++] = ch;
            currentCol++;

            // Handle optional + or - after exponent
            char next = fgetc(fp);
            if (next == '+' || next == '-') {
                buffer[index++] = next;
                currentCol++;
            } else {
                ungetc(next, fp); // Not sign: push back
            }

            // Next must be a digit
            next = fgetc(fp);
            if (!isdigit(next)) {
                fprintf(stderr, "Lexical Error [Line %d, Col %d]: Exponent missing digits\n",
                        currentLine, currentCol);
                break;
            }
            buffer[index++] = next;
            currentCol++;
        }
        else {
            // End of number: push back the character for later
            ungetc(ch, fp);
            currentCol--;
            break;
        }
        if (index >= (MAX_LEN - 1)) {
            fprintf(stderr, "Lexical Error: Decimal number too long\n");
            break;
        }
    }

    buffer[index] = '\0';

    // Ending check for incomplete number
    if (buffer[index - 1] == '.' || buffer[index - 1] == 'e' || buffer[index - 1] == 'E' ||
        buffer[index - 1] == '+' || buffer[index - 1] == '-') {
        fprintf(stderr, "Lexical Error [Line %d, Col %d]: Invalid number termination: %s\n",
                currentLine, currentCol, buffer);
    } else {
        printf("Decimal Number: %s\n", buffer);
    }

    memset(buffer, 0, index);
}

// Determines the type of numeric literal and dispatches to appropriate handler
void handleDigit(FILE *fp, char *buffer, char ch) {
    // Case: number starts with '0'
    if (ch == '0') {
        char next = fgetc(fp);

        // Hexadecimal
        if (next == 'x' || next == 'X') {
            handleHex(fp, buffer, next);
            return;
        }
        // Octal
        else if (next == 'o' || next == 'O') {
            handleOctal(fp, buffer, next);
            return;
        }
        // Binary
        else if (next == 'b' || next == 'B') {
            handleBinary(fp, buffer, next);
            return;
        }

        // Not a prefix: push back and treat as decimal
        if (next != EOF)
            ungetc(next, fp);
    }

    // Not a '0'-prefix or fallback to decimal
    handleDecimal(fp, buffer, ch);
}
