#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 80
#define MAX_SYMBOL_LENGTH 31

// Structure for symbol table nodes
typedef struct Symbol {
    char name[MAX];
    int value;
    char properties[3][MAX];
    struct Symbol *next;
} Symbol;

// Structure for the instruction image linked list
typedef struct InstructionNode {
    char instruction[25];  // Each instruction word in 24-bit binary (plus null-terminator)
    int L;
    int Address;
    struct InstructionNode *next;
} InstructionNode;

// Structure for the instruction image linked list
typedef struct DataNode {
    int value;  // The binary value or ASCII value to be stored
    char binaryRep[25];
    struct DataNode *next;  // Pointer to the next node
} DataNode;

typedef struct Macro {
    char name[MAX];
    char content[MAX * MAX];
    struct Macro *next;
} Macro;

typedef struct Opcode {
    char name[MAX];         // Opcode name
    int value;              // Opcode value
    int numOfOperands;      // Number of operands
    int funct;              // The funct field (additional property)
    struct Opcode *next;    // Pointer to the next opcode in the list
} Opcode;

// Define a structure to store valid addressing modes for an opcode
typedef struct {
    char name[MAX];       // Opcode name
    int validSourceModes[4];  // Valid addressing modes for the source operand
    int validTargetModes[4];  // Valid addressing modes for the target operand
} OpcodeAddressingModes;

typedef struct ExternalReference {
    char symbolName[MAX];  // Name of the external symbol
    int address;           // The address in the code where the symbol is used
    struct ExternalReference *next;  // Pointer to the next reference in the list
} ExternalReference;

// Function declarations

// Opcode List management
void insertOpcode(char *name, int value, int numOfOperands, int funct);
void populateOpcodeList();
Opcode *findOpcode(char *opcodeName);

// Symbol Table management
void insertSymbol(Symbol **head, char *name, int value, char properties[3][MAX]);
void addSymbolToTable(char *name, int value, char *prop1, char *prop2, char *prop3);
Symbol *findSymbol(char *symbolName);  

// Instruction List management
void addInstruction(char *binaryInstruction, int L);
void updateInstruction(int position, char *newInstruction);
void printInstructionList();

// Data List management
void insertData(int value, int *DC, char *binaryOfInt);
void printDataList();
void updateDataSymbols(Symbol *head);

// External References management
void addExternalReference(char *symbolName, int address);
void printExternalReferences();

#endif
