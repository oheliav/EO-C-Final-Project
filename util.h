#ifndef UTIL_H
#define UTIL_H
#include "dataStructures.h"
#include "globals.h"


// Checks if a string has the format of a symbol (one word that ends with ':')
int isSymbolFormat(char *word);

// Checks if a given word is an opcode by looking it up in the opcode list
int isOpcode(char *word);

// Returns the line without the first word (label or opcode)
char *skipFirstWord(char *line);

// Checks if a string represents a valid integer
int isValidInteger(const char *str);

// Checks if a given word is a reserved keyword
int isReservedWord(const char *word);

// Checks if a symbol name exists either in the symbol table or macro table
int isExistingSymbolOrMacro(const char *symbolName);

// Validates an operand symbol (it may already exist)
int isValidOperandSymbol(const char *symbol);

// Validates a new symbol, ensuring it's not a reserved word or already existing
int isValidSymbol(const char *symbol);

// Skips commas and ensures correct placement of commas between labels
int skipComma(char **linePtr, int commaRequired);

// Parses an ".extern" directive line and returns the labels found in the line
char **parseExternLine(const char *line, int *numLabels);

// Calculates the number of words (L) for the machine code based on addressing modes
int calculateL(Opcode *opcode, int mode1, int mode2);

// Checks if a string is a valid index (must be a non-negative integer)
int isValidIndex(const char *str);

// Converts an integer (0-3) into a 2-bit binary string
char *intTo2BitBinary(int num);

// Converts an integer (0-15) into a 6-bit binary string
char *intTo6BitBinary(int num);

char *intTo5BitBinary(int num);

char *intTo3BitBinary(int num);

char *intTo24BitBinary(int num);

// Checks if a given operand is a valid register name
int isRegisterName(const char *operand);

// Parses a .data directive line and stores the values in the data image
void parseData(const char *dataContent, int *DC);

// Parses an opcode line, processes the operands, and returns the number of words (L)
int parseOpcodeLine(char *line);

// Parses a .string directive line and stores the string values in the data image
void parseString(const char *stringContent, int *DC);

int isExternal(Symbol *symbol);

void printEntrySymbols();

char *encodeImmediateOperand(char *operand);

#endif // UTIL_H
