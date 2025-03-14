#include "dataStructures.h"
#include "globals.h"
#include "preAssembler.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Linked list for opcodes
Opcode *opcodeList = NULL;
Symbol *symbolTable = NULL;
Macro *macroTable = NULL;  // Head of the Macros table
InstructionNode *instructionList = NULL; // Head of the instruction list (global)
DataNode *dataList = NULL;  // Head of the linked list
ExternalReference *externalReferencesList = NULL;  // Head of the external references list

// Define other global variables (IC, DC, etc.)
int IC = 0;
int DC = 0;
int foundError = 0;
int counter;
int ICF;
int IDF;
const char *reservedWords[] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", 
    "jmp", "bne", "red", "prn", "jsr", "rts", "stop", 
    "define", "data", "string", "extern", "entry", 
    NULL // End of the list
};
const char *registerNames[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", NULL
};

// Array to store valid addressing modes for each opcode
OpcodeAddressingModes validAddressingModes[] = {
    {"mov", {0, 1, 3, -1}, {1, 3, -1, -1}},  // Source: 0, 1, 3; Target: 1, 3
    {"cmp", {0, 1, 3, -1}, {0, 1, 3, -1}},  // Source: 0, 1, 3; Target: 0, 1, 3
    {"add", {0, 1, 3, -1}, {1, 3, -1, -1}},  // Source: 0, 1, 3; Target: 1, 3
    {"sub", {0, 1, 3, -1}, {1, 3, -1, -1}},  // Source: 0, 1, 3; Target: 1, 3
    {"lea", {1, -1, -1, -1}, {1, 3, -1, -1}},  // Source: 1; Target: 1, 3
    {"clr", {-1, -1, -1, -1}, {1, 3, -1, -1}},  // No source; Target: 1, 3
    {"not", {-1, -1, -1, -1}, {1, 3, -1, -1}},  // No source; Target: 1, 3
    {"inc", {-1, -1, -1, -1}, {1, 3, -1, -1}},  // No source; Target: 1, 3
    {"dec", {-1, -1, -1, -1}, {1, 3, -1, -1}},  // No source; Target: 1, 3
    {"jmp", {-1, -1, -1, -1}, {1, 2, -1, -1}},  // No source; Target: 1, 2
    {"bne", {-1, -1, -1, -1}, {1, 2, -1, -1}},  // No source; Target: 1, 2
    {"jsr", {-1, -1, -1, -1}, {1, 2, -1, -1}},  // No source; Target: 1, 2
    {"red", {-1, -1, -1, -1}, {1, 3, -1, -1}},  // No source; Target: 1, 3
    {"prn", {-1, -1, -1, -1}, {0, 1, 3, -1}},  // No source; Target: 0, 1, 3
    {"rts", {-1, -1, -1, -1}, {-1, -1, -1, -1}},  // No source; No target
    {"stop", {-1, -1, -1, -1}, {-1, -1, -1, -1}}  // No source; No target
};

