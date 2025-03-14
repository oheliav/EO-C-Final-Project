#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

// Function declarations

// Function that runs the first pass
int firstPass(char *filename);

/*
// Function to update data symbols in the symbol table after the first pass
void updateDataSymbols(Symbol *head, int ICF);
*/

// Function to determine the line type
int determineLineType(char *line);

// Function to process a line from the source file
void processLine(char *line, int *IC, int *DC);  // Pass IC and DC by reference

// Function to generate the first word of machine code as a binary string
char *generateFirstWord(Opcode *opcode, int mode1, int mode2, int registerNum1, int registerNum2);

// Function to find the addressing mode of an operand
int findAddressingMode(char *operand);

/*
// Function to raise an error message
void raiseError(char *errorMsg);*/

#endif /* FIRSTPASS_H */
