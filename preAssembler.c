#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preAssembler.h"
#include "globals.h"
#include "errors.h"

/* Function to ignore empty lines or lines with only spaces */
int ignorePre(char *line) {
    int i, length;
    
    /* Store the length once to avoid repeated calls to strlen */
    length = (int)strlen(line);

    for (i = 0; i < length; i++) {
        if (!isspace((unsigned char)line[i])) {
            return 0;  /* Not a whitespace-only line */
        }
    }
    return 1;  /* Skip whitespace-only lines */
}

// Function to check if a macro name is valid (starts with a letter, can include alphanumeric and underscores)
int isValidMacroName(char *macroName) {
    // Step 1: Check if the first character is a letter
    if (!isalpha(macroName[0])) {
        return 0;  // Invalid: must start with a letter
    }

    // Step 2: Check that the remaining characters are either letters, digits, or underscores
    for (int i = 1; macroName[i] != '\0'; i++) {
        if (!isalnum(macroName[i]) && macroName[i] != '_') {
            return 0;  // Invalid character found
        }
    }

    return 1;  // Valid macro name
}

// Function to check if a macro name is a reserved word or already declared as a macro
int isReservedOrDeclared(char *macroName) {
    // Step 1: Check if the name is a reserved word
    for (int i = 0; reservedWords[i] != NULL; i++) {
        if (strcmp(macroName, reservedWords[i]) == 0) {
            return 1;  // Macro name is a reserved word
        }
    }

    for (int i = 0; registerNames[i] != NULL; i++) {
        if (strcmp(macroName, registerNames[i]) == 0) {
            return 1; // Macro name is a register name
        }
    }

    // Step 3: Check if the name is already declared as a macro
    Macro *current = macroTable;
    while (current != NULL) {
        if (strcmp(current->name, macroName) == 0) {
            return 1;  // Macro name is already declared
        }
        current = current->next;
    }

    return 0;  // Name is neither reserved nor declared
}

// Function to check if a line starts with a valid macro initialization ("mcro nameOfMacro")
int isMacroInitialization(char line[]) {
    char macroName[MAX];

    // Step 1: Check if "mcro" exists in the line
    char *mcroPosition = strstr(line, "mcro");
    if (mcroPosition == NULL) {
        return 0;  // Not a macro initialization line, so we don't care
    }

    // Step 2: Avoid treating "mcroend" as a macro initialization
    if (strstr(line, "mcroend") != NULL) {
        return 0;  // It's a macro end, not a macro initialization
    }

    // Step 3: Check if there are any characters before "mcro"
    if (mcroPosition != line) {
        raiseError("Error: Characters found before 'mcro' at the beginning of the macro definition in line %d.\n", counter);
        return 0;
    }

    // Step 4: Check if "mcro" is followed directly by a macro name without space (e.g., "mcroMacroName")
    if (line[4] != ' ') {  // There must be a space immediately after "mcro"
        raiseError("Error: 'mcro' and the macro name cannot be connected in line %d. Use a space after 'mcro'.\n", counter);
        return 0;
    }

    // Step 5: Extract the macro keyword and name
    int scannedFields = sscanf(line, "mcro %s", macroName);

    // Step 6: Check if we correctly extracted the macro name
    if (scannedFields != 1) {
        raiseError("Error: Macro name is missing or invalid in line: %d\n", counter);
        return 0;
    }

    // Step 7: Validate the macro name (starts with a letter, contains alphanumeric or underscores)
    if (!isValidMacroName(macroName)) {
        raiseError("Error: Macro name '%s' in line %d is invalid. Must start with a letter and contain only alphanumeric characters or underscores.\n", macroName, counter);
        return 0;
    }

    // Step 8: Check if the macro name is a reserved word or already declared
    if (isReservedOrDeclared(macroName)) {
        raiseError("Error: Macro name '%s' is either a reserved word or already declared.\n", macroName);
        return 0;
    }

    // Step 9: Check if there are extra characters after the macro name
    char *remaining = strstr(line, macroName) + strlen(macroName);
    
    // Skip any whitespace
    while (*remaining == ' ' || *remaining == '\t') {
        remaining++;
    }

    if (*remaining != '\0' && *remaining != '\n') {
        raiseError("Error: Extra characters found after macro name: %s\n", line);
        return 0;
    }

    // If everything is valid
    printf("Valid macro initialization: mcro %s\n", macroName); // Debug line
    return 1;
}

// Function to check if a line contains "mcroend" and handle invalid characters before/after
int isEndMacro(char *line) {
    // Step 1: Check if "mcroend" exists in the line
    char *endMacroPosition = strstr(line, "mcroend");
    if (endMacroPosition == NULL) {
        return 0;  // "mcroend" not found, return false
    }

    // Step 2: Check if there are characters before "mcroend"
    if (endMacroPosition != line) {
        raiseError("Error: Characters found before 'mcroend' in line %d.\n", counter);
        return 0;
    }

    // Step 3: Move the pointer past the "mcroend" keyword
    char *remaining = endMacroPosition + strlen("mcroend");

    // Skip any whitespace
    while (*remaining == ' ' || *remaining == '\t') {
        remaining++;
    }

    // Step 4: Check if there are any characters after "mcroend"
    if (*remaining != '\0' && *remaining != '\n') {
        raiseError("Error: Extra characters found after 'mcroend' in line %d.\n", counter);
        return 0;
    }

    // If everything is valid
    return 1;  // It's a valid "mcroend" line
}

/**
 * @brief Adds a macro to the macro table (linked list).
 * 
 * @param head Pointer to the head of the macro linked list.
 * @param name Pointer to the macro name string.
 * @param content Pointer to the macro content string.
 * @return Macro* Returns a pointer to the updated macro table.
 */
Macro *addToMacroTable(Macro *head, char *name, char *content) {
    Macro *newMacro;
    Macro *current;

    /* Allocate memory for the new macro */
    newMacro = (Macro *)malloc(sizeof(Macro));
    if (newMacro == NULL) {
        printf("Error allocating memory for new macro.\n");
        return head;
    }

    /* Copy macro name and content */
    strcpy(newMacro->name, name);
    strcpy(newMacro->content, content);
    newMacro->next = NULL;

    /* If the list is empty, return the new macro as the head */
    if (head == NULL) {
        return newMacro;
    }

    /* Traverse to the last macro in the list */
    current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* Append the new macro at the end */
    current->next = newMacro;

    return head;
}

/**
 * @brief Parses and stores a macro name from a line.
 * 
 * @param head Pointer to the head of the macro linked list.
 * @param line Pointer to the input line containing the macro definition.
 * @return Macro* Returns a pointer to the updated macro table.
 */
Macro *insertMacroName(Macro *head, char *line) {
    char name[MAX];

    /* Extract the macro name from the line (skip "mcro" keyword) */
    sscanf(line, "%*s %s", name);

    /* Add the macro to the macro table with an empty content field */
    return addToMacroTable(head, name, "");
}

/**
 * @brief Appends content to the last macro in the linked list.
 * 
 * @param head Pointer to the head of the macro linked list.
 * @param line Pointer to the content line to be added.
 */
void insertMacroContent(Macro *head, char *line) {
    Macro *current;
    size_t len;

    current = head; /* Declare at the top, per ISO C90 */

    /* Traverse to the last macro in the list */
    while (current->next != NULL) {
        current = current->next;
    }

    /* Append the new content */
    strcat(current->content, line);

    /* Ensure exactly one newline at the end */
    len = strlen(current->content);
    while (len > 0 && current->content[len - 1] == '\n') {
        current->content[--len] = '\0';
    }
    strcat(current->content, "\n");
}

/**
 * @brief Checks if the given name matches any macro name in the linked list.
 * 
 * @param head Pointer to the head of the macro linked list.
 * @param name The macro name to search for.
 * @return int Returns 1 if the macro name exists, 0 otherwise.
 */
int isMacroName(Macro *head, char *name) {
    Macro *current;
    current = head; 

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/**
 * @brief Removes trailing newline characters from a string.
 * 
 * @param str Pointer to the string to be trimmed.
 */
void trimTrailingNewline(char *str) {
    int len;
    
    len = strlen(str);
    while (len > 0 && str[len - 1] == '\n') {
        str[--len] = '\0';
    }
}

/**
 * @brief Replaces a macro name with its corresponding content.
 * 
 * @param head Pointer to the head of the macro linked list.
 * @param macroName Pointer to the macro name to search for.
 * @param replacedLine Pointer to the buffer where the macro content will be copied.
 */
void replaceMacro(Macro *head, char *macroName, char *replacedLine) {
    Macro *current;

    current = head; /* Declare at the top, per ISO C90 */

    while (current != NULL) {
        if (strcmp(current->name, macroName) == 0) {
            strcpy(replacedLine, current->content);  /* Replace with the macro content */
            
            /* Use a helper function to trim newlines */
            trimTrailingNewline(replacedLine);
            
            return;
        }
        current = current->next;
    }
}

// Main function to process the file, replacing macros with their content
void processFile(char *inputFile, char *outputFile, Macro *head) {
    FILE *fpInput = fopen(inputFile, "r");
    FILE *fpOutput = fopen(outputFile, "w");
    char line[MAX];
    int isMacro = 0;

    if (fpInput == NULL) {
        printf("Error: Unable to open file %s\n", inputFile);
        return;
    }

    if (fpOutput == NULL) {
        printf("Error: Unable to create file %s\n", outputFile);
        fclose(fpInput);
        return;
    }

    while (fgets(line, MAX, fpInput)) {
        line[strcspn(line, "\n")] = '\0';  // Remove newline character
        counter++;

        // Remove leading whitespaces from all lines
        char *trimmedLine = line;
        while (isspace(*trimmedLine)) {
            trimmedLine++;
        }

        if (ignorePre(trimmedLine)) {
            continue;  // Skip empty lines
        }

        if (isMacro) {  // Inside a macro definition
            if (isEndMacro(trimmedLine)) {
                isMacro = 0;  // End macro definition
                continue;
            } else {
                insertMacroContent(head, trimmedLine);
                continue;
            }
        }

        if (isMacroInitialization(trimmedLine)) {  // Start of a new macro
            head = insertMacroName(head, trimmedLine);
            isMacro = 1;
            continue;
        }

        char replacedLine[MAX] = "";
        if (isMacroName(head, trimmedLine)) {
            replaceMacro(head, trimmedLine, replacedLine);
            fprintf(fpOutput, "%s\n", replacedLine);  // Write macro content without extra newlines
        } else {
            fprintf(fpOutput, "%s\n", trimmedLine);  // Write regular line without extra newlines
        }
    }

    fclose(fpInput);
    fclose(fpOutput);
}

// Function to free the macro table memory
void freeMacroTable(Macro *head) {
    Macro *current = head;
    while (current != NULL) {
        Macro *next = current->next;
        free(current);
        current = next;
    }
}

// Entry point: Pass input and output files to the pre-assembler
int preAssembler(char *inputFileName, char *outputFileName) {
    printf("Running pre-assembler on input: %s, output: %s\n", inputFileName, outputFileName);

    // Initialize macro table to NULL
    Macro *macroTable = NULL;

    // Initialize counter to 0, it will start from 1 in the process file funct.
    counter = 0;

    // Process the input file and write the result to the output file
    processFile(inputFileName, outputFileName, macroTable);

    // Free the memory used by the macro table
    freeMacroTable(macroTable);

    if (foundError == 1){
        return 0;
    } 
    return 1;
}
