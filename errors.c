#include <stdio.h>
#include <stdarg.h>
#include "error.h"
#include "globals.h"

// Function to raise and print an error message
void raiseError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // Set the foundError flag
    foundError = 1;
    
    // Print the formatted error message
    printf("Error: ");
    vprintf(format, args);
    printf("\n");

    va_end(args);
}

void raiseErrorPreAssembler(){

}