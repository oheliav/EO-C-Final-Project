#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX 80
#define MAX_SYMBOL_LENGTH 31
#define BINARY_LENGTH 25

// Include the full structure definitions from dataStructures.h
#include "dataStructures.h"

// Addressing Modes
#define IMMEDIATE 0  // Immediate addressing (#value)
#define DIRECT 1     // Direct addressing (label)
#define RELATIVE 2      // Relative addressing (&label)
#define REGISTER 3   // Direct register addressing (r0-r7)

// Declare global variables as extern

// Typedef-based variables
extern Macro *macroTable;                // Macro table (linked list of macros)
extern Symbol *symbolTable;              // Symbol table (linked list of symbols)
extern Opcode *opcodeList;               // Linked list of opcodes
extern InstructionNode *instructionList; // Linked list of instruction words
extern DataNode *dataList;               // Linked list of data words
extern OpcodeAddressingModes validAddressingModes[]; // Valid addressing modes for opcodes
extern ExternalReference *externalReferencesList;    // List of external references

// Integers
extern int foundError;   // Error flag to indicate if any errors were found
extern int DC;           // Data counter
extern int IC;           // Instruction counter
extern int ICF;
extern int IDF;
extern int counter;      // Line counter

// Character pointers
extern const char *registerNames[];      // List of register names (e.g., "r0" to "r7")
extern const char *reservedWords[];      // List of reserved words in the language


#endif
