#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"
#include "util.h"      // Include the corresponding header file for declarations
#include "globals.h"   // Include globals for shared constants and global variables
#include "firstPass.h"
#include "errors.h"
#include "bitUtils.h"

// Add your utility function implementations here

// Function that checks if a word is a symbol format ('label:')
int isSymbolFormat(char *word) {
    int length = strlen(word);
    
    // Ensure the word is not empty and ends with a colon
    if (length > 0 && word[length - 1] == ':') {
        return 1;  // Valid symbol format
    }
    
    return 0;  // Not a valid symbol format
}

// Going over the opcodes table to check if an opcode is legit
int isOpcode(char *word) {
    Opcode *current = opcodeList;  // Global linked list from globals.c
    while (current != NULL) {
        // Debugging statement with proper format specifiers        
        if (strcmp(current->name, word) == 0) {
            return 1;  // Opcode found
        }
        current = current->next;
    }
    return 0;  // Opcode not found
}

// Returns the line without the first word (label or opcode)
char *skipFirstWord(char *line) {
    char *rest = strchr(line, ' ');  // Find the first space after the first word
    if (rest == NULL) {
        return "";  // No space found, return empty string
    }
    while (*rest == ' ') {
        rest++;  // Skip additional spaces after the first word
    }
    return rest;
}

//check
// Checks if a number is a valid integer
int isValidInteger(const char *str) {
    // Check if the string is empty
    if (str == NULL || *str == '\0') {
        return 0;  // Invalid input
    }

    // Check for an optional sign at the beginning
    if (*str == '-' || *str == '+') {
        str++;  // Skip the sign
    }

    // Ensure there are digits after the sign (or at the start if no sign)
    if (!isdigit(*str)) {
        return 0;  // Invalid: must start with a digit
    }

    // Check the rest of the string
    while (*str) {
        if (!isdigit(*str)) {
            return 0;  // Invalid: contains non-digit characters (e.g., '.', letters)
        }
        str++;
    }

    return 1;  // Valid integer
}

//check
// Checks if a word is a language reserved word
int isReservedWord(const char *word) {
    for (int i = 0; reservedWords[i] != NULL; i++) {
        if (strcmp(word, reservedWords[i]) == 0) {
            return 1;  // The word is a reserved keyword
        }
    }
    return 0;  // Not a reserved keyword
}

// Checks if a symbol name is an existing symbol or existing macro
int isExistingSymbolOrMacro(const char *symbolName) {
    // Check if the symbol is in the symbol table
    Symbol *currentSymbol = symbolTable;
    while (currentSymbol != NULL) {
        if (strcmp(currentSymbol->name, symbolName) == 0) {
            return 1;  // Symbol already exists
        }
        currentSymbol = currentSymbol->next;
    }

    // Check if the symbol is in the macro table (assuming you have a macro structure)
    Macro *currentMacro = macroTable;
    while (currentMacro != NULL) {
        if (strcmp(currentMacro->name, symbolName) == 0) {
            return 1;  // Macro already exists
        }
        currentMacro = currentMacro->next;
    }

    return 0;  // The symbol or macro doesn't exist
}

/**
 * @brief Checks if an operand symbol is valid.
 * 
 * @param symbol Pointer to the operand symbol string.
 * @return int Returns 1 if valid, 0 if invalid.
 */
int isValidOperandSymbol(const char *symbol) {
    int length;
    int i;

    /* Get the symbol length */
    length = strlen(symbol);

    /* Step 1: Check if the symbol exceeds the maximum length */
    if (length > MAX_SYMBOL_LENGTH) {  
        return 0;  /* Invalid: symbol is too long */
    }

    /* Step 2: Check that the first character is alphabetic */
    if (!isalpha((unsigned char)symbol[0])) {
        printf("Operand symbol '%s' fails at Step 2: Must start with an alphabetic letter\n", symbol);
        return 0;  /* Invalid: must start with a letter */
    }

    /* Step 3: Check that the rest of the characters are alphanumeric */
    for (i = 1; i < length; i++) {
        if (!isalnum((unsigned char)symbol[i])) {
            printf("Operand symbol '%s' fails at Step 3: Must be alphanumeric\n", symbol);
            return 0;  /* Invalid: must be alphanumeric */
        }
    }

    printf("Operand symbol '%s' is valid\n", symbol);
    return 1;  /* Valid operand */
}

// Function that checks if a symbol is valid
int isValidSymbol(const char *symbol) {
    int length = strlen(symbol);
    char symbolName[MAX_SYMBOL_LENGTH + 1];

    // Step 1: Check if the symbol exceeds the maximum length
    if (length > MAX_SYMBOL_LENGTH + 1) {  // +1 to account for the colon
        raiseError("Symbol is longer than max length\n");
        return 0;  // Invalid: symbol is too long
    }

    // Step 2: Check if the symbol ends with a colon (':')
    if (symbol[length - 1] == ':') {
        // Remove the trailing colon for further validation
        strncpy(symbolName, symbol, length - 1);  // Copy the symbol without the colon
        symbolName[length - 1] = '\0';
        length--;  // Adjust length to exclude colon
    } else {
        // Copy the symbol as-is, since there's no colon
        strcpy(symbolName, symbol);
    }

    // Step 3: Check that the first character is alphabetic
    if (!isalpha(symbolName[0])) {
        printf("Line %d: Symbol '%s' fails at Step 3 - Must start with an alphabetic letter\n", counter, symbolName);
        return 0;  // Invalid: must start with a letter
    }

    // Step 4: Check that the rest of the characters are alphanumeric
    for (int i = 1; i < length; i++) {  // Use updated length
        if (!isalnum(symbolName[i])) {
            printf("Line %d: Symbol '%s' fails at Step 4: Must be alphanumeric\n", counter, symbolName);
            return 0;  // Invalid: must be alphanumeric
        }
    }

    // Step 5: Check if the symbol is a reserved word
    if (isReservedWord(symbolName)) {
        printf("Line %d: Symbol '%s' fails at Step 5: It's a reserved word\n", counter, symbolName);
        return 0;  // Invalid: symbol is a reserved word
    }

    // Step 6: Check if the symbol already exists in the symbol or macro table
    if (isExistingSymbolOrMacro(symbolName)) {
        printf("Line %d: Symbol '%s' fails at Step 6: It already exists either as a label either a macro\n", counter, symbolName);
        return 0;  // Invalid: symbol already exists
    }

    // Step 7: If all checks passed, the symbol is valid
    printf("Symbol '%s' is valid\n", symbolName); //Debug line
    return 1;
}

// Helper function to skip commas and check if we need a comma
int skipComma(char **linePtr, int commaRequired) {
    char *line = *linePtr;
    while (isspace((unsigned char)*line)) line++;  // Skip whitespace

    if (commaRequired) {
        if (*line != ',') {
            printf("Error: Expected a comma between labels.\n");
            return 0;  // Comma was required but not found
        }
        line++;  // Skip the comma
    }

    // Skip any whitespace after the comma
    while (isspace((unsigned char)*line)) line++;

    *linePtr = line;  // Update the line pointer after processing
    return 1;  // Success
}

// Function to parse the .extern line and return an array of labels
char **parseExternLine(const char *line, int *numLabels) {

    char *lineCopy = strdup(line);  // Create a modifiable copy of the line
    char *linePtr = lineCopy;
    char label[MAX];
    int count = 0;
    int commaRequired = 0;  // No comma required before the first label

    // Move the pointer past the ".extern" keyword
    linePtr += strlen(".extern");

    // Allocate memory for an array of strings (labels)
    char **labels = (char **)malloc(MAX * sizeof(char *));
    if (labels == NULL) {
        printf("Memory allocation error!\n");
        free(lineCopy);
        return NULL;
    }

    // Parse the labels after ".extern"
    while (*linePtr != '\0') {
        // Skip initial spaces and tabs
        while (*linePtr == ' ' || *linePtr == '\t') {
            linePtr++;
        }

        // Extract the label
        if (sscanf(linePtr, "%[^, \t\n]", label) != 1) {
            printf("Error: Expected a label.\n");
            free(lineCopy);
            for (int i = 0; i < count; i++) {
                free(labels[i]);
            }
            free(labels);
            return NULL;  // Error parsing label
        }

        // Check if label is valid and add it to the array
        if (isValidSymbol(label)) {
            labels[count] = strdup(label);
            if (labels[count] == NULL) {
                printf("Memory allocation error!\n");
                free(lineCopy);
                for (int i = 0; i < count; i++) {
                    free(labels[i]);
                }
                free(labels);
                return NULL;
            }
            count++;
        }

        // Move the linePtr past the current label
        linePtr += strlen(label);

        // Skip trailing spaces
        while (*linePtr == ' ' || *linePtr == '\t') {
            linePtr++;
        }

        // Check if there is a comma after the label
        if (*linePtr == ',') {
            linePtr++;  // Move past the comma
            commaRequired = 1;  // Now we require commas between subsequent labels
        } else if (commaRequired) {
            // If we expected a comma but did not find it
            printf("Error: Expected a comma between labels.\n");
            free(lineCopy);
            for (int i = 0; i < count; i++) {
                free(labels[i]);
            }
            free(labels);
            return NULL;
        }
    }

    *numLabels = count;  // Set the number of parsed labels
    free(lineCopy);
    return labels;  // Return the array of labels
}


int calculateL(Opcode *opcode, int mode1, int mode2) {
    int L = 1;  // Start with 1 for the first word of the machine code

    // Check the number of operands
    if (opcode->numOfOperands == 1) {
        // One operand: check the addressing mode of mode2 (since it's the target operand)
        switch (mode2) {
            case IMMEDIATE:  // Immediate addressing
                L += 1;  // Needs 1 extra word for the immediate value
                break;
            case DIRECT:  // Direct addressing (label)
                L += 1;  // Needs 1 extra word for the label address
                break;
            case RELATIVE:  // Index addressing (label[index])
                L += 1;  // Needs 1 extra words: one for the label and one for the index
                break;
            case REGISTER:  // Register addressing
                L += 0;  // Adds 0 extra word for register addressing (for 1 operand)
                break;
        }
    } else if (opcode->numOfOperands == 2) {
       
        // First operand (mode1)
        switch (mode1) {
            case IMMEDIATE:
                L += 1;  // Needs 1 extra word for the immediate value
                break;
            case DIRECT:
                L += 1;  // Needs 1 extra word for the label address
                break;
            case RELATIVE:
                L += 1;  // Needs 2 extra words for the label and index
                break;
            case REGISTER:
                L += 0;  // Adds 1 extra word for register addressing (if not both are registers)
                break;
        }

        // Second operand (mode2)
        switch (mode2) {
            case IMMEDIATE:
                L += 1;  // Needs 1 extra word for the immediate value
                break;
            case DIRECT:
                L += 1;  // Needs 1 extra word for the label address
                break;
            case RELATIVE:
                L += 1;  // Needs 2 extra words for the label and index
                break;
            case REGISTER:
                L += 0;  // Adds 1 extra word for register addressing (if not both are registers)
                break;
        }
    }
    return L;  // Return the total number of machine code words
}

// Function to check if a string is a valid index (non-negative integer)
int isValidIndex(const char *str) {
    // Step 1: Check if the string starts with a plus sign or is a digit
    if (*str == '+') {
        str++;  // Skip the plus sign
    }

    // Step 2: Ensure there are digits following the plus sign (or starting immediately if no plus sign)
    if (!isdigit(*str)) {
        return 0;  // Invalid: Must start with a digit after optional plus sign
    }

    // Step 3: Check the rest of the string to ensure it contains only digits
    while (*str) {
        if (!isdigit(*str)) {
            return 0;  // Invalid: Non-digit character found
        }
        str++;
    }

    // If all checks passed, it's a valid index
    return 1;
}
/*
// Function to convert an integer (0-63) to a 6-bit binary string and return it
char *intTo6BitBinary(int num) {
    // Allocate memory for the binary string (6 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(7 * sizeof(char));
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string, filling MSB to LSB correctly
    for (int i = 5; i >= 0; i--) {
        binaryStr[5 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[6] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-3) to a 2-bit binary string and return it
char *intTo2BitBinary(int num) {
    // Allocate memory for the binary string (2 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(3 * sizeof(char));  // 2 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 1; i >= 0; i--) {
        binaryStr[1 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[2] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-31) to a 5-bit binary string and return it
char *intTo5BitBinary(int num) {
    // Allocate memory for the binary string (5 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(6 * sizeof(char));  // 5 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 4; i >= 0; i--) {
        binaryStr[4 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[5] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-7) to a 3-bit binary string and return it
char *intTo3BitBinary(int num) {
    // Allocate memory for the binary string (3 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(4 * sizeof(char));  // 3 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 2; i >= 0; i--) {
        binaryStr[2 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[3] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}
*/
int isRegisterName(const char *operand) {
    char trimmedOperand[MAX] = "";
    
    // Using sscanf to skip leading spaces and extract the operand
    sscanf(operand, "%s", trimmedOperand);  // This will effectively remove leading/trailing spaces
    
    int i = 0;
    while (registerNames[i] != NULL) {
        if (strcmp(trimmedOperand, registerNames[i]) == 0) {
            return 1;  // Valid register name
        }
        i++;
    }
    return 0;  // Invalid register name
}

void parseData(const char *dataContent, int *DC) {
    char *token = strtok((char *)dataContent, ",");

    while (token != NULL) {
        // Manually trim leading and trailing whitespace
        while (isspace((unsigned char)*token)) token++;  // Skip leading whitespace
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;  // Skip trailing whitespace
        *(end + 1) = '\0';  // Null-terminate after trimming

        int value;
        if (isValidInteger(token)) {
            value = atoi(token);  // Parse the integer value
            printf("Found integer: %d\n", value);
        } else {
            printf("Error: Invalid data in .data directive.\n");
            return;
        }
        char *bin = intTo24BitBinary(value);
        // Insert the value into the linked list
        insertData(value, DC, bin);

        // Get the next token
        token = strtok(NULL, ",");
    }
}

// Helper function to check if a mode is valid for the operand
int isValidAddressingMode(int mode, const int validModes[]) {
    for (int i = 0; validModes[i] != -1; i++) {
        if (mode == validModes[i]) {
            return 1;  // Mode is valid
        }
    }
    return 0;  // Mode is not valid
}

// Function to check if the addressing modes are valid for the given opcode
int checkValidAddressingMode(Opcode *opcode, int sourceMode, int targetMode) {
    for (int i = 0; i < 16; i++) {
        // Find the corresponding opcode in the validAddressingModes array
        if (strcmp(validAddressingModes[i].name, opcode->name) == 0) {
            
            // If the opcode has 0 operands, no validation is needed
            if (opcode->numOfOperands == 0) {
                return 1;  // Valid, nothing to check
            }

            // If the opcode has 1 operand, only check the target operand
            if (opcode->numOfOperands == 1) {
                if (isValidAddressingMode(targetMode, validAddressingModes[i].validTargetModes)) {
                    return 1;  // Valid target addressing mode
                } else {
                    printf("Error: Invalid addressing mode for target operand in opcode '%s'.\n", opcode->name);
                    return 0;  // Invalid target addressing mode
                }
            }

            // If the opcode has 2 operands, check both source and target operands
            if (opcode->numOfOperands == 2) {
                int sourceValid = isValidAddressingMode(sourceMode, validAddressingModes[i].validSourceModes);
                int targetValid = isValidAddressingMode(targetMode, validAddressingModes[i].validTargetModes);
                
                // If both are valid, return 1
                if (sourceValid && targetValid) {
                    return 1;
                }
                
                // If one or both are invalid, print an error and return 0
                if (!sourceValid) {
                    printf("Error: Invalid addressing mode for source operand in opcode '%s'.\n", opcode->name);
                }
                if (!targetValid) {
                    printf("Error: Invalid addressing mode for target operand in opcode '%s'.\n", opcode->name);
                }
                return 0;  // Addressing mode is not valid
            }
        }
    }

    // Opcode not found (this shouldn't happen if opcodes are properly populated)
    printf("Error: Opcode '%s' not found in addressing mode validation.\n", opcode->name);
    return 0;
}
// Function to convert an immediate operand (e.g., #5) into a 24-bit binary string
char *encodeImmediateOperand(char *operand) {
    int value = atoi(operand + 1);  // Skip the '#' character to get the immediate value

    // Allocate memory for the 24-bit binary string (+1 for null terminator)
    char *binaryStr = (char *)malloc(25 * sizeof(char));
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert the immediate value into a 21-bit binary string
    for (int i = 20; i >= 0; i--) {
        binaryStr[i] = (value & 1) ? '1' : '0';  // Extract the least significant bit
        value >>= 1;  // Right shift the value to get the next bit
    }

    // Set the A,R,E bits to '100' (absolute addressing)
    binaryStr[21] = '1';  // A bit (absolute)
    binaryStr[22] = '0';  // R bit
    binaryStr[23] = '0';  // E bit
    binaryStr[24] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Skeleton for parsing an opcode line
int parseOpcodeLine(char *line) {
    char label[MAX_SYMBOL_LENGTH] = "";  // Optional label
    char opcodeName[MAX_SYMBOL_LENGTH] = "";
    char operand1[MAX_SYMBOL_LENGTH] = "";  // Source operand
    char operand2[MAX_SYMBOL_LENGTH] = "";  // Target operand
    int numOperands = 0;
    sscanf(line, "%s", label);
    
    // Step 1: Check if there is a label (label must end with ':')
    if (isSymbolFormat(label)) {
        line = skipFirstWord(line);
    } 
    // Step 2: Extract the opcode
    sscanf(line, "%s", opcodeName); 
    printf("This is the opecode name: %s\n", opcodeName);
    Opcode *opcode = findOpcode(opcodeName); //step 12
    if (opcode == NULL) {
        printf("Error20: Unknown opcode '%s'.\n", opcodeName);
        return 1;
    }
    printf("starting step 3 with opcode %s\n", opcode->name);
    // Step 3: Check the number of operands based on the opcode
    numOperands = opcode->numOfOperands;
    int mode1 = -1, mode2 = -1;
    //check here if there's enough and not more than the operands required
    if (numOperands == 2) {
        // Extract source and target operands, trimming leading whitespaces for operand1 and operand2
        sscanf(line + strlen(opcodeName), " %[^,], %s", operand1, operand2);
        printf("found 2 operands: %s, %s\n", operand1, operand2);
        mode1 = findAddressingMode(operand1);  // Addressing mode for source operand
        mode2 = findAddressingMode(operand2);  // Addressing mode for target operand

    } else if (numOperands == 1) {
        // Extract single operand
        sscanf(line + strlen(opcodeName), "%s", operand2);  // Operand2 is target
        mode2 = findAddressingMode(operand2);  // Addressing mode for target operand
    }
    // CHECK THE ADDRESSING MODES FOR THE OPCODE, RAISE ERRORS
    int test = checkValidAddressingMode(opcode, mode1, mode2);
    if (!test) {
        printf("Invalid adressing mode for opcode: %s\n", opcode->name);
    }

    int L = 1;
    printf("Mode 1 is: %d, mode2 is: %d\n", mode1, mode2);
    L = calculateL(opcode, mode1, mode2);
    printf("L is %d\n", L);
    int reg1 = -1, reg2 = -1;

    if (mode1 == REGISTER) {
        reg1 = operand1[1] - '0';  // Convert the character to an integer (register number)
    }

    if (mode2 == REGISTER) {
        reg2 = operand2[1] - '0';  // Convert the character to an integer (register number)
    }
    printf("Starting to decode first word of machine code for the instruction\n");
    // Step 5: Generate the first word of machine code (as a string)
    char *firstWord = generateFirstWord(opcode, mode1, mode2, reg1, reg2); //step 14
    // Output the generated machine code (as a binary string)
    printf("First word of machine code: %s\n", firstWord);
    addInstruction(firstWord, L);

    if (mode1 == IMMEDIATE) {
        char *binaryImmidiate = encodeImmediateOperand(operand1);
        updateInstruction(IC + 1, binaryImmidiate);  // IC + 1 because first word is opcode
        printf("Operand 1 is immediate addressing, binary machine code is: %s\n", binaryImmidiate);
        free(binaryImmidiate);  // Don't forget to free the dynamically allocated memory
    }

    if (mode2 == IMMEDIATE) {
        int position = (numOperands == 1) ? IC + 1 : IC + 2;  // Adjust for the number of operands
        char *binaryImmidiate = encodeImmediateOperand(operand2);
        updateInstruction(position, binaryImmidiate);  // IC + 2 if 2 operands
        printf("Operand 2 is immediate addressing, binary machine code is: %s\n", binaryImmidiate);
        free(binaryImmidiate);  // Free the allocated memory
    }

    // Free the allocated memory for the binary string
    free(firstWord);
    return L;
}
/*
char *intTo24BitBinary(int num) {
    // Allocate memory for the binary string (24 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(25 * sizeof(char));  // 24 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to 24-bit two's complement binary string
    for (int i = 23; i >= 0; i--) {
        binaryStr[23 - i] = (num & (1 << i)) ? '1' : '0';  // Check if bit i is set
    }
    binaryStr[24] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}
*/
void parseString(const char *stringContent, int *DC) {

    // Process each character in the string
    while (*stringContent) {
        int asciiValue = (int)(*stringContent);
        printf("Storing ASCII value of '%c': %d\n", *stringContent, asciiValue);
        char *bin = intTo24BitBinary(asciiValue);
        // Insert the ASCII value into the linked list
        insertData(asciiValue, DC, bin);

        stringContent++;
    }

    // Insert the null terminator '\0'
    insertData(0, DC, "00000000000000000000000");  // Null-terminate the string
}

int isExternal(Symbol *symbol) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(symbol->properties[i], "external") == 0) {
            return 1;  // Symbol is external
        }
    }
    return 0;  // Not an external symbol
}

void printEntrySymbols() {
    Symbol *current = symbolTable;
    printf("Entry Symbols:\n");
    while (current != NULL) {
        if (strcmp(current->properties[2], "entry") == 0) {
            printf("Symbol: %s, Value: %d\n", current->name, current->value);
        }
        current = current->next;
    }
}