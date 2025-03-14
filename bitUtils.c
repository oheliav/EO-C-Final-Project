#include <stdio.h>
#include <stdlib.h>
#include "bitUtils.h"

// Function to convert a binary string to a hexadecimal string
char *binaryToHexString(const char *binary) {
    unsigned int decimal = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        decimal = (decimal << 1) + (binary[i] - '0'); // Binary to decimal conversion
    }

    char *hexString = (char *)malloc(7 * sizeof(char)); // 6 digits + null terminator
    if (hexString == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    snprintf(hexString, 7, "%06x", decimal); // Format as six-digit hexadecimal
    return hexString;
}

// Function to convert an integer (0-63) to a 6-bit binary string and return it
char *intTo6BitBinary(int num) {
    // Allocate memory for the binary string (6 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(7 * sizeof(char));
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string, filling MSB to LSB correctly
    for (int i = 5; i >= 0; i--) {
        binaryStr[5 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[6] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-3) to a 2-bit binary string and return it
char *intTo2BitBinary(int num) {
    // Allocate memory for the binary string (2 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(3 * sizeof(char));  // 2 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 1; i >= 0; i--) {
        binaryStr[1 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[2] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-31) to a 5-bit binary string and return it
char *intTo5BitBinary(int num) {
    // Allocate memory for the binary string (5 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(6 * sizeof(char));  // 5 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 4; i >= 0; i--) {
        binaryStr[4 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[5] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

// Function to convert an integer (0-7) to a 3-bit binary string and return it
char *intTo3BitBinary(int num) {
    // Allocate memory for the binary string (3 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(4 * sizeof(char));  // 3 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to binary string (MSB first)
    for (int i = 2; i >= 0; i--) {
        binaryStr[2 - i] = (num & (1 << i)) ? '1' : '0';  // Extract the i-th bit
    }
    binaryStr[3] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}

char *intTo24BitBinary(int num) {
    // Allocate memory for the binary string (24 bits + 1 for null terminator)
    char *binaryStr = (char *)malloc(25 * sizeof(char));  // 24 bits + null terminator
    
    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }

    // Convert integer to 24-bit two's complement binary string
    for (int i = 23; i >= 0; i--) {
        binaryStr[23 - i] = (num & (1 << i)) ? '1' : '0';  // Check if bit i is set
    }
    binaryStr[24] = '\0';  // Null-terminate the string

    return binaryStr;  // Return the binary string
}