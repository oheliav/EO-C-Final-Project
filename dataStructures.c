#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "dataStructures.h"


/**
 * @brief Inserts an opcode into the opcode linked list.
 * 
 * @param name Pointer to the opcode name.
 * @param value Numeric value assigned to the opcode.
 * @param numOfOperands Number of operands the opcode requires.
 * @param funct Additional function code for the opcode.
 */
void insertOpcode(char *name, int value, int numOfOperands, int funct) {
    Opcode *newOpcode;

    /* Allocate memory for a new opcode */
    newOpcode = (Opcode *)malloc(sizeof(Opcode));
    if (newOpcode == NULL) {
        printf("Memory allocation failed for opcode '%s'.\n", name);
        return;
    }
    
    /* Set the opcode fields */
    strcpy(newOpcode->name, name);
    newOpcode->value = value;
    newOpcode->numOfOperands = numOfOperands;
    newOpcode->funct = funct;
    newOpcode->next = NULL;

    /* Insert at the beginning of the linked list */
    newOpcode->next = opcodeList;
    opcodeList = newOpcode;
}


/**
 * @brief Populates the opcode list with predefined opcodes.
 */
void populateOpcodeList() {
    /* Insert each opcode with its corresponding value, number of operands, and funct value */
    insertOpcode("mov", 0, 2, -1);     /* No funct */
    insertOpcode("cmp", 1, 2, -1);     /* No funct */
    insertOpcode("add", 2, 2, 1);      /* funct = 1 */
    insertOpcode("sub", 2, 2, 2);      /* funct = 2 */
    insertOpcode("lea", 4, 2, -1);     /* No funct */
    insertOpcode("clr", 5, 1, 1);      /* funct = 1 */
    insertOpcode("not", 5, 1, 2);      /* funct = 2 */
    insertOpcode("inc", 5, 1, 3);      /* funct = 3 */
    insertOpcode("dec", 5, 1, 4);      /* funct = 4 */
    insertOpcode("jmp", 9, 1, 1);      /* funct = 1 */
    insertOpcode("bne", 9, 1, 2);      /* funct = 2 */
    insertOpcode("jsr", 9, 1, 3);      /* funct = 3 */
    insertOpcode("red", 12, 1, -1);    /* No funct */
    insertOpcode("prn", 13, 1, -1);    /* No funct */
    insertOpcode("rts", 14, 0, -1);    /* No funct */
    insertOpcode("stop", 15, 0, -1);   /* No funct (stop) */
}


// Function to insert a symbol into the symbol table
void insertSymbol(Symbol **head, char *name, int value, char properties[3][MAX]) {
    // Remove the colon at the end of the symbol name, if present
    int len = strlen(name);
    if (name[len - 1] == ':') {
        name[len - 1] = '\0';  // Remove the trailing colon
    }

    // Check if the symbol already exists in the table
    Symbol *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Error: Symbol '%s' already exists in the table.\n", name);
            return;
        }
        current = current->next;
    }

    // Create a new symbol
    Symbol *newSymbol = (Symbol *)malloc(sizeof(Symbol));
    if (newSymbol == NULL) {
        printf("Memory allocation error!\n");
        return;
    }

    // Set the symbol's name, value, and properties
    strcpy(newSymbol->name, name);
    newSymbol->value = value;
    for (int i = 0; i < 3; i++) {
        strcpy(newSymbol->properties[i], properties[i]);
    }
    newSymbol->next = NULL;

    // Append the new symbol at the end of the list (instead of inserting at the head)
    if (*head == NULL) {
        // If the list is empty, set the new symbol as the head
        *head = newSymbol;
    } else {
        // Traverse to the end of the list and append the new symbol
        Symbol *last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newSymbol;
    }

    printf("Symbol '%s' added to the table.\n", name);
}


// Adds a symbol to symbols table using insertSymbol func
void addSymbolToTable(char *name, int value, char *prop1, char *prop2, char *prop3) {
    char properties[3][MAX];  // Array to store the final properties

    // Step 1: Assign the provided properties or empty strings if NULL
    strcpy(properties[0], (prop1 != NULL) ? prop1 : "");
    strcpy(properties[1], (prop2 != NULL) ? prop2 : "");
    strcpy(properties[2], (prop3 != NULL) ? prop3 : "");

    // Step 2: Insert the symbol into the table with the final properties
    insertSymbol(&symbolTable, name, value, properties);
}

/**
 * @brief Finds an opcode in the linked list by name.
 * 
 * @param opcodeName Pointer to the name of the opcode to search for.
 * @return Opcode* Pointer to the matching opcode, or NULL if not found.
 */
Opcode *findOpcode(char *opcodeName) {
    Opcode *current;

    /* Start at the head of the linked list */
    current = opcodeList;
    
    /* Traverse the linked list */
    while (current != NULL) {
        /* Compare the given opcodeName with the current node's opcode name */
        if (strcmp(current->name, opcodeName) == 0) {
            printf("Returning: %s\n", current->name);
            return current;  /* Return the pointer to the matching opcode */
        }
        current = current->next;  /* Move to the next node in the list */
    }

    return NULL;  /* Return NULL if no matching opcode is found */
}


// Function to insert an instruction into the list
void addInstruction(char *binaryInstruction, int L) {
    // Allocate memory for the new instruction node
    InstructionNode *newNode = (InstructionNode *)malloc(sizeof(InstructionNode));
    
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        return;
    }

    // Copy the binary instruction to the new node
    strcpy(newNode->instruction, binaryInstruction);
    newNode->L = L;
    newNode->Address = IC;
    newNode->next = NULL;

    // If the list is empty, set this node as the first one
    if (instructionList == NULL) {
        instructionList = newNode;
    } else {
        // Traverse to the end of the list to append the new instruction
        InstructionNode *current = instructionList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    // Update the current pointer to the newly added instruction
    InstructionNode *current = newNode;

    // If L > 1, add '?' placeholders for the extra words
    for (int i = 1; i < L; i++) {
        InstructionNode *placeholderNode = (InstructionNode *)malloc(sizeof(InstructionNode));
        if (placeholderNode == NULL) {
            printf("Memory allocation error for placeholder\n");
            return;
        }
        strcpy(placeholderNode->instruction, "???????????????????????");
        placeholderNode->Address = IC + i;
        placeholderNode->next = NULL;

        current->next = placeholderNode;  // Link the placeholder node
        current = placeholderNode;  // Move to the new placeholder node
    }
}

// Function to update a specific placeholder with the correct instruction
void updateInstruction(int position, char *newInstruction) {
    InstructionNode *current = instructionList;

    // Traverse the list to find the node at 'position'
    while (current != NULL && current->Address != position) {
        current = current->next;
    }

    // Once the correct position is found, check if the placeholder is "??????..."
    if (current != NULL && strcmp(current->instruction, "???????????????????????") == 0) {
        // Update the instruction
        strcpy(current->instruction, newInstruction);
    } else {
        printf("Error: No placeholder at the specified position\n");
    }
}

//Only for debugging
void printInstructionList() {
    InstructionNode *current = instructionList;
    while (current != NULL) {
        printf("IC: %d  Instruction: %s\n", current->Address, current->instruction);
        current = current->next;
    }
}

// Insert ASCII characters into dataArray from a string
void insertData(int value, int *DC, char *binaryOfInt) {
    DataNode *newNode = (DataNode *)malloc(sizeof(DataNode));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        return;
    }

    strcpy(newNode->binaryRep, binaryOfInt);
    newNode->value = value;
    newNode->next = NULL;

    if (dataList == NULL) {
        dataList = newNode;
    } else {
        DataNode *current = dataList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    (*DC)++;  // Increment DC for each value added
}

//Only for debugging 
void printDataList() {
    DataNode *current = dataList;
    int index = 0;
    while (current != NULL) {
        printf("Data[%d]: %d, binary: %s\n", index++, current->value, current->binaryRep);
        current = current->next;
    }
}

/**
 * @brief Finds a symbol in the symbol table by name.
 * 
 * @param symbolName Pointer to the name of the symbol to search for.
 * @return Symbol* Pointer to the matching symbol, or NULL if not found.
 */
Symbol *findSymbol(char *symbolName) {
    Symbol *current;

    /* Start at the head of the symbol table */
    current = symbolTable;

    /* Traverse the linked list */
    while (current != NULL) {
        /* Compare the given symbolName with the current node's symbol name */
        if (strcmp(current->name, symbolName) == 0) {
            return current;  /* Symbol found */
        }
        current = current->next;  /* Move to the next node in the list */
    }

    return NULL;  /* Symbol not found */
}

// Function to add an external reference to the list
void addExternalReference(char *symbolName, int address) {
    // Allocate memory for a new external reference
    ExternalReference *newReference = (ExternalReference *)malloc(sizeof(ExternalReference));
    if (newReference == NULL) {
        printf("Memory allocation failed for external reference\n");
        exit(1);
    }

    // Set the symbol name and address
    strcpy(newReference->symbolName, symbolName);
    newReference->address = address;
    newReference->next = NULL;

    // Insert the new reference at the end of the list
    if (externalReferencesList == NULL) {
        externalReferencesList = newReference;  // First element in the list
    } else {
        ExternalReference *current = externalReferencesList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newReference;  // Insert at the end of the list
    }
}

// Function to print the external references (or save to a file)
void printExternalReferences() {
    ExternalReference *current = externalReferencesList;
    while (current != NULL) {
        printf("External symbol '%s' used at address %d\n", current->symbolName, current->address);
        current = current->next;
    }
}

// Function to update each of the data symbols in the symbols table after the first pass, for step 17
void updateDataSymbols(Symbol *head) {
    Symbol *current = head;

    while (current != NULL) {
        // Check if the symbol has the "data" property
        for (int i = 0; i < 3; i++) {
            if (strcmp(current->properties[i], "data") == 0) {
                // Update the value by adding
                current->value += (ICF);
                printf("Updated symbol '%s' with new value %d.\n", current->name, current->value);
                break;  // No need to check the other properties
            }
        }
        current = current->next;  // Move to the next symbol in the table
    }
}