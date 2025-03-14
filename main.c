#include "globals.h"
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"  
#include "errors.h"
#include "main.h"
#include "bitUtils.h"
#include <stdio.h>

// Function to create the .ob object file
void createObjectFile(char *baseFile) {
    char obFileName[MAX];
    snprintf(obFileName, sizeof(obFileName), "%s.ob", baseFile);

    FILE *obFile = fopen(obFileName, "w");
    if (obFile == NULL) {
        raiseError("Error: Unable to create object file: %s\n", obFileName);
        return;
    }

    // Write the first line: instruction count and data count
    fprintf(obFile, "%d %d\n", ICF - 100, IDF);

    // Write the instructions
    InstructionNode *currentInstruction = instructionList;
    int address = currentInstruction->Address;
    while (currentInstruction != NULL) {
        char *hexValue = binaryToHexString(currentInstruction->instruction);
        fprintf(obFile, "%07d %s\n", currentInstruction->Address, hexValue);
        free(hexValue); // Free allocated memory for hexadecimal string
        currentInstruction = currentInstruction->next;
        address++;
    }

    // Write the data
    DataNode *currentData = dataList;
    while (currentData != NULL) {
        char *hexValue = binaryToHexString(currentData->binaryRep);
        fprintf(obFile, "%07d %s\n", address, hexValue);
        free(hexValue); // Free allocated memory for hexadecimal string
        currentData = currentData->next;
        address++;
    }

    fclose(obFile);
    printf("Object file created: %s\n", obFileName);
}

void createEntryFile(char *baseFile) {
    char entFileName[MAX];
    snprintf(entFileName, sizeof(entFileName), "%s.ent", baseFile);
    
    FILE *entFile = fopen(entFileName, "w");
    if (entFile == NULL) {
        raiseError("Error: Unable to create entry file: %s\n", entFileName);
        return;
    }

    Symbol *current = symbolTable;  // Traverse the symbol table to find entry symbols
    int entriesFound = 0;

    while (current != NULL) {
        // Check if the symbol has the 'entry' property
        printf("printing inside createEntries - current symbol: %s\n", current->name);
        if (strcmp(current->properties[2], "entry") == 0) {
            // Write the symbol name and its value in 7-digit format
            fprintf(entFile, "%s %07d\n", current->name, current->value);
            entriesFound++;
        }
        current = current->next;
    }

    fclose(entFile);

    if (entriesFound > 0) {
        printf("Entry file created: %s\n", entFileName);
    } else {
        // No entry symbols found, so delete the file
        remove(entFileName);
        printf("No entry symbols found. Entry file not created.\n");
    }
}

void createExternalFile(char *baseFile) {
    char extFileName[MAX];
    snprintf(extFileName, sizeof(extFileName), "%s.ext", baseFile);

    FILE *extFile = fopen(extFileName, "w");
    if (extFile == NULL) {
        raiseError("Error: Unable to create external file: %s\n", extFileName);
        return;
    }

    // Traverse the external references list and write each entry to the file
    ExternalReference *current = externalReferencesList;
    while (current != NULL) {
        // Write the symbol name and its address to the file
        fprintf(extFile, "%s %07d\n", current->symbolName, current->address);
        current = current->next;  // Move to the next external reference
    }

    fclose(extFile);
    printf("External file created: %s\n", extFileName);
}

int main(int argc, char *argv[]) {
    // Step 0: Populate the opcode list
    populateOpcodeList();

    // Step 1: Validate arguments
    if (argc < 2) {
        printf("Usage: %s <input_file_1> <input_file_2> ... <input_file_n>\n", argv[0]);
        return 1;
    }

    // Step 2: Loop through each base file name provided as argument
    for (int i = 1; i < argc; i++) {
        char *baseFile = argv[i];
        char inputFile[MAX];    // To store the .asm file name
        char outputFile[MAX];   // To store the .am file name

        // Step 3: Create the input file name by appending .asm to the base file name
        snprintf(inputFile, sizeof(inputFile), "%s.asm", baseFile);

        // Check if the input file exists
        FILE *file = fopen(inputFile, "r");
        if (file == NULL) {
            printf("Error: Input file '%s' does not exist or cannot be accessed.\n", inputFile);
            continue;  // Move to the next file
        }

        // Check if the file is empty
        fseek(file, 0, SEEK_END);  // Move to the end of the file
        if (ftell(file) == 0) {    // Check if the position is 0 (empty file)
            printf("Error: Input file '%s' is empty.\n", inputFile);
            fclose(file);
            continue;  // Move to the next file
        }
        fclose(file);

        // Step 4: Create the output file name by appending .am to the base file name
        snprintf(outputFile, sizeof(outputFile), "%s.am", baseFile);

        // Step 5: Run the pre-assembler
        printf("Running the pre-assembler on %s...\n", inputFile);
        if (preAssembler(inputFile, outputFile)) {
            printf("Pre-assembler completed successfully. Output written to: %s\n", outputFile);
        } else {
            printf("Pre-assembler found errors in %s, does not continue to first pass.\n", inputFile);
            cleanupAssembler();
            continue;  // Move on to the next file if pre-assembler fails
        }

        // Step 6: Run the first pass using the output of the pre-assembler
        printf("Running the first pass on %s...\n", outputFile);
        if (!firstPass(outputFile)) {
            printf("First pass failed for %s.\n", outputFile);
            cleanupAssembler();
            printf("Moving to the next file\n");
            continue;  // Move on to the next file if the first pass fails
        }

        printf("First pass completed successfully for %s.\n", outputFile);
        printInstructionList();
        printDataList();

        // Step 7: Run the second pass
        printf("Running the second pass on %s...\n", outputFile);
        secondPass(outputFile);

        if (foundError) {
            printf("Second pass failed for %s.\n", outputFile);
            cleanupAssembler();
            continue;  // Move on to the next file if the second pass fails
        }

        printf("Second pass completed successfully for %s.\n", outputFile);
        printInstructionList();
        printDataList();

        // Step 8: Create the object file
        createObjectFile(baseFile);

        // Step 9: Conditionally create the entry and external files
        if (hasEntrySymbols()) {
            createEntryFile(baseFile);
        }

        if (hasExternalReferences()) {
            createExternalFile(baseFile);
        }
        cleanupAssembler();
    }

    printf("Assembler completed processing all files.\n");
    return 0;
}

// Check if there are any entry symbols in the symbol table
int hasEntrySymbols() {
    Symbol *current = symbolTable;  // Start with the head of the symbol table

    while (current != NULL) {
        // Check if the symbol has the 'entry' property
        if (strcmp(current->properties[2], "entry") == 0) {
            return 1;  // Found an entry symbol
        }
        current = current->next;  // Move to the next symbol in the list
    }

    return 0;  // No entry symbols found
}

// Check if there are any external references in the external references list
int hasExternalReferences() {
    ExternalReference *current = externalReferencesList;  // Start with the head of the external references list

    // Traverse the external references list
    while (current != NULL) {
        return 1;  // As soon as we find an external reference, return 1
        current = current->next;
    }

    return 0;  // No external references found
}

// Function to clean up the assembler's data structures
void cleanupAssembler() {
    // Free the symbol table
    Symbol *currentSymbol = symbolTable;
    while (currentSymbol != NULL) {
        Symbol *temp = currentSymbol;
        currentSymbol = currentSymbol->next;
        free(temp);
    }
    symbolTable = NULL;

    // Free the macro table
    Macro *currentMacro = macroTable;
    while (currentMacro != NULL) {
        Macro *temp = currentMacro;
        currentMacro = currentMacro->next;
        free(temp);
    }
    macroTable = NULL;

    // Free the instruction list
    InstructionNode *currentInstruction = instructionList;
    while (currentInstruction != NULL) {
        InstructionNode *temp = currentInstruction;
        currentInstruction = currentInstruction->next;
        free(temp);
    }
    instructionList = NULL;

    // Free the data list
    DataNode *currentData = dataList;
    while (currentData != NULL) {
        DataNode *temp = currentData;
        currentData = currentData->next;
        free(temp);
    }
    dataList = NULL;

    // Free external references list
    ExternalReference *currentExternalRef = externalReferencesList;
    while (currentExternalRef != NULL) {
        ExternalReference *temp = currentExternalRef;
        currentExternalRef = currentExternalRef->next;
        free(temp);
    }
    externalReferencesList = NULL;

    // Reset error flag, data counter (DC), and instruction counter (IC)
    foundError = 0;
    DC = 0;
    IC = 0;
    counter = 0;

    // Any other cleanup if needed (e.g., resetting specific flags or state variables)
}
