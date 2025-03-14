#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "dataStructures.h"
#include "util.h"
#include "firstPass.h"
#include "secondPass.h"
#include "errors.h"
#include "bitUtils.h"



void secondPass(char *sourceFile) {
    // Step 1: Initiate IC
    IC = 100;
    FILE *file = fopen(sourceFile, "r");
    if (file == NULL) {
        printf("Error: Could not open source file: %s\n", sourceFile);
        return;
    }
    
    char line[MAX];
    
    // Step 2: Read each line from the source file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Step 3-8: Process each line using the processLineSecondPass function
        processLineSecondPass(line);
    }
    
    // Step 9: After reading the file, check for errors
    if (foundError) {
        printf("Errors found during the second pass.\n");
        fclose(file);
        return;
    }
    
    // Step 10: Build the output files (this will be handled later)
    fclose(file);
    printf("Second pass completed successfully.\n");
}

void processLineSecondPass(char *line) {
    printf("processing line: %s\n", line);
    char firstWord[MAX];
    sscanf(line, "%s", firstWord);
    int num = determineLineType(line);
    int L = 0;
    printf("line type: %d\n", num);

    // Step 3: Check if the first word is a symbol
    if (isSymbolFormat(firstWord)) {
        // Skip the symbol and get the next word
        line = skipFirstWord(line);
        sscanf(line, "%s", firstWord);
    }
    printf("first word now is: %s\n", firstWord); // Debug line

    // Step 4: Skip .data, .string, and directives
    if (num == 0 || num == 1) {
        printf("Skip second pass\n");
        return;  // Skip this line, return to secondPass
    }
    if (num == 2) {
        // Step 5: Handle .entry directive
        if (strstr(firstWord, ".entry") != NULL) {  // Use strstr to be more flexible
            parseEntryLine(line);
            return;
        } else { // .extern
            return;
        }
    }

    printf("Trying to debug\n");
    // Step 6: Decoding operands (This is where we'll decode the second word and beyond)
    // Find symbols in the symbol table, raise error if not found.
    // This part will be expanded later with operand decoding logic.
    if (num == 3){ //That means it's an opcode line
        printf("Opcode line\n");
        L = parseOpcodeSecondPass(line);
    }
    // Step 8: Update IC
    IC += L;
}

void parseEntryLine(const char *line) {
    char *lineCopy = strdup(line);  // Create a modifiable copy of the line
    char *linePtr = lineCopy;
    char symbolName[MAX];
    int commaRequired = 0;  // No comma required before the first label

    // Move the pointer past the ".entry" keyword and skip leading whitespace
    linePtr = strstr(linePtr, ".entry") + strlen(".entry");
    while (*linePtr == ' ' || *linePtr == '\t') {
        linePtr++;  // Skip spaces and tabs
    }

    // Parse the labels after ".entry"
    while (*linePtr != '\0') {
        if (!skipComma(&linePtr, commaRequired)) {
            free(lineCopy);
            return;  // Error with commas
        }

        // Extract the label
        if (sscanf(linePtr, "%[^, \t\n]", symbolName) != 1) {
            printf("Error: Expected a label.\n");
            free(lineCopy);
            return;  // Error parsing label
        }

        printf("Checking symbol: %s\n", symbolName);

        // Find the symbol in the symbol table
        Symbol *symbol = findSymbol(symbolName);  // Function to find a symbol in the table
        if (symbol) {
            // Mark the symbol as 'entry'
            strcpy(symbol->properties[2], "entry");
            printf("Symbol '%s' marked as 'entry'\n", symbolName);
        } else {
            printf("Error: Symbol '%s' not found in symbol table.\n", symbolName);
        }

        // Move the linePtr past the current label
        linePtr += strlen(symbolName);

        // Skip any trailing whitespace
        while (*linePtr == ' ' || *linePtr == '\t') {
            linePtr++;
        }

        // Set commaRequired to 1 after a label is parsed (except the last one)
        commaRequired = 1;
    }

    free(lineCopy);
}

// Handle Direct Addressing Mode (label)
void handleDirectAddressing(char *operand, int position) {
    // Find the symbol in the symbol table
    Symbol *symbol = findSymbol(operand);
    
    // If the symbol is not found, raise an error and return
    if (symbol == NULL) {
        printf("Error: Symbol '%s' not found\n", operand);
        return;
    }

    // Allocate memory for a 24-bit binary string (+1 for the null terminator)
    char *binaryWord = (char *)malloc(25 * sizeof(char));
    if (binaryWord == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Initialize the binaryWord with 0s and null-terminate the string
    memset(binaryWord, '0', 24);
    binaryWord[24] = '\0';

    // Get the symbol value
    int value = symbol->value;

    // Convert the symbol value into a 21-bit binary string and store it in the leftmost 21 bits
    for (int i = 20; i >= 0; i--) {
        binaryWord[i] = (value & 1) ? '1' : '0';  // Extract the least significant bit
        value >>= 1;  // Right shift the number to get the next bit
    }

    // Set the last 3 bits (A, R, E)
    if (isExternal(symbol)) {
        binaryWord[21] = '0';  // A = 0
        binaryWord[22] = '0';  // R = 0
        binaryWord[23] = '1';  // E = 1 (external symbol)

        // Record the external symbol usage
        addExternalReference(operand, position);

    } else {
        binaryWord[21] = '0';  // A = 0
        binaryWord[22] = '1';  // R = 1 (relocatable symbol)
        binaryWord[23] = '0';  // E = 0
    }

    // Update the instruction at the given position
    updateInstruction(position, binaryWord);

    // Free the allocated memory for the binaryWord
    free(binaryWord);
}

// Handle Relative Addressing Mode (&label)
void handleRelativeAddressing(char *operand, int position) {
    // Ensure the operand starts with '&'
    if (operand[0] != '&') {
        printf("Error: Invalid operand format for relative addressing: '%s'\n", operand);
        return;
    }

    // Extract the label (skip the '&' character)
    char *label = operand + 1;

    // Find the symbol in the symbol table
    Symbol *symbol = findSymbol(label);
    
    // Ensure the symbol exists and is not external
    if (symbol == NULL) {
        printf("Error: Symbol '%s' not found\n", label);
        return;
    }
    if (isExternal(symbol)) {
        printf("Error: Symbol '%s' is external, relative addressing cannot be used with external symbols\n", label);
        return;
    }

    // Calculate the relative distance (label address - current position)
    int distance = symbol->value - (position-1);
    printf("Symbol value is %d, position is %d, distance is %d\n", symbol->value, position-1, distance);
    // Convert the distance to 21-bit binary
    char *binaryWord = (char *)malloc(25 * sizeof(char));  // 24 bits + null terminator
    if (binaryWord == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    memset(binaryWord, '0', 24);  // Initialize all to '0'
    binaryWord[24] = '\0';  // Null-terminate the string

    //Placeholder: create a function that converts integer to 21bit binary

    // Convert the distance to a 21-bit signed binary and store it in the leftmost 21 bits
    for (int i = 20; i >= 0; i--) {
        binaryWord[i] = (distance & 1) ? '1' : '0';  // Extract each bit from the distance
        distance >>= 1;  // Shift right to get the next bit
    }

    // Set the last 3 bits to '100' (absolute addressing for relative mode)
    binaryWord[21] = '1';  // A = 1
    binaryWord[22] = '0';  // R = 0
    binaryWord[23] = '0';  // E = 0

    // Update the instruction at the given position with the new machine code
    updateInstruction(position, binaryWord);

    // Free allocated memory
    free(binaryWord);
}

// Main function to decode the operand and update the machine code based on the addressing mode
void decodeOperandToMachineCode(char *operand, int addressingMode, int position) {
    printf("Decoding operand: '%s' at position: %d with addressing mode: %d\n", operand, position, addressingMode);
    switch (addressingMode) {
        case IMMEDIATE:
            // Already decoded in first pass
            break;
        case DIRECT:
            handleDirectAddressing(operand, position);
            break;
        case RELATIVE:
            handleRelativeAddressing(operand, position);
            break;
        case REGISTER:
            // Already decoded in first pass
            break;
        default:
            printf("Error: Unknown addressing mode %d\n", addressingMode);
            break;
    }
    return;
}

int parseOpcodeSecondPass(char *line) {
    char label[MAX_SYMBOL_LENGTH] = "";  // Optional label
    char opcodeName[MAX_SYMBOL_LENGTH] = "";
    char operand1[MAX_SYMBOL_LENGTH] = "";  // Source operand
    char operand2[MAX_SYMBOL_LENGTH] = "";  // Target operand
    int numOperands = 0;

    sscanf(line, "%s", label);

    // Step 1: Check if there is a label
    if (isSymbolFormat(label)) {
        line = skipFirstWord(line);
        printf("testing\n");
    }

    // Step 2: Extract the opcode
    sscanf(line, "%s", opcodeName);
    Opcode *opcode = findOpcode(opcodeName);
    if (opcode == NULL) {
        raiseError("Error: Unknown opcode '%s'.\n", opcodeName);
        return 1;
    }

    printf("Found Opcode: %s\n", opcodeName);

    // Step 3: Check the number of operands based on the opcode
    int mode1 = -1, mode2 = -1;
    int L = 1;  // Start with 1 for the first word

    if (opcode->numOfOperands == 2) {
        sscanf(line + strlen(opcodeName), " %[^,], %s", operand1, operand2);
        printf("Operands: '%s', '%s'\n", operand1, operand2);
        mode1 = findAddressingMode(operand1);  // Addressing mode for source operand
        mode2 = findAddressingMode(operand2);  // Addressing mode for target operand
        printf("Addressing modes: mode1=%d, mode2=%d\n", mode1, mode2);

    } else if (opcode->numOfOperands == 1) {
        sscanf(line + strlen(opcodeName), "%s", operand2);  // Operand2 is target
        printf("Operand: '%s'\n", operand2);
        mode2 = findAddressingMode(operand2);  // Addressing mode for target operand
        printf("Addressing mode: mode2=%d\n", mode2);
    }

    L = calculateL(opcode, mode1, mode2);
    printf("Number of words (L): %d\n", L);

    // Start decoding the operands from the second word to L
    /*int position = 1;  // Start at the second word (index 1, as index 0 is the first word)*/
    if (opcode->numOfOperands == 2) {
        decodeOperandToMachineCode(operand1, mode1, IC + 1);  // Decode first operand
        if (mode1 == REGISTER) {
            decodeOperandToMachineCode(operand2, mode2, IC + 1);  // Decode second operand
        } else {
            decodeOperandToMachineCode(operand2, mode2, IC + 2);  // Decode second operand
        }
    } else if (opcode->numOfOperands == 1) {
        decodeOperandToMachineCode(operand2, mode2, IC + 1);  // Decode single operand
    }

    return L;  // Return the number of words decoded
}

