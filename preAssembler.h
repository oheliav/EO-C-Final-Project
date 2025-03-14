
#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H
#include "globals.h"
#include "dataStructures.h"

int ignorePre(char *line);
Macro *addToMacroTable(Macro *head, char name[], char content[]);
void printMacroTable(Macro *head);
int isMacroName(Macro *head, char *name);
int isMacroInitialization(char line[]);
int isReservedOrDeclared(char *macroName);
int isValidMacroName(char *macroName);
Macro* insertMacroName(Macro* head, char line[]);
void insertMacroContent(Macro *head, char line[]);
void processFile(char *inputFile, char *outputFile, Macro *head);
int isEndMacro(char line[]);
void replaceMacro(Macro *head, char *macroName, char *replacedLine);
void writeFile(char *inputFileName, Macro *head);
int preAssembler(char *inputFileName, char *outputFileName);
void trimTrailingNewline(char *str);

#endif 
