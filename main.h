#ifndef MAIN_H
#define MAIN_H

#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "error.h"
#include "globals.h"

// Function declarations for main.c

// Function to check if any entry symbols exist in the symbol table
int hasEntrySymbols();

// Function to check if there are any external references in the external reference list
int hasExternalReferences();

// Function to create the output object file (.ob)
void createObjectFile(char *baseFile);

// Function to create the entry file (.ent) if entry symbols exist
void createEntryFile(char *baseFile);

// Function to create the external reference file (.ext) if external references exist
void createExternalFile(char *baseFile);

// Main function that manages the assembling process
int main(int argc, char *argv[]);

void cleanupAssembler();

#endif // MAIN_H
