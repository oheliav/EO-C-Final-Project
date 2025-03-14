#ifndef SECONDPASS_H
#define SECONDPASS_H

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "dataStructures.h"
#include "util.h"
#include "firstPass.h"

// Function to initiate the second pass of the assembler
void secondPass(char *sourceFile);

// Function to process each line during the second pass
void processLineSecondPass(char *line);

// Function to parse .entry lines and update the symbol table with the "entry" property
void parseEntryLine(const char *line);

// Helper function to convert a value into a 12-bit binary string for machine code generation
void convertToBinary(int value, char *binaryWord, int start, int length);

// Functions to handle various addressing modes
void handleDirectAddressing(char *operand, int wordIndex);
void handleRelativeAddressing(char *operand, int position);

// Function to decode an operand and update the machine code based on the addressing mode
void decodeOperandToMachineCode(char *operand, int addressingMode, int position);  // Corrected signature

// Function to parse and process opcode lines in the second pass
int parseOpcodeSecondPass(char *line);

#endif // SECONDPASS_H
