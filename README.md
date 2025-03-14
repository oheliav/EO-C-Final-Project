# EO-C-Final-Project

## 🛠️ Overview
This project is a **two-pass assembler** written in **C** that processes assembly-like source files, converts them into machine code, and generates output files. It includes a **pre-assembler**, **symbol table handling**, **instruction and data storage**, and **error management**.

## 🔥 Features
- **Pre-Assembler**: Handles macros and expands them before processing.
- **Two-Pass Assembly**:
  - **First Pass**: Builds the symbol table and processes labels.
  - **Second Pass**: Generates final machine code and resolves symbols.
- **Supports Directives**: `.data`, `.string`, `.entry`, `.extern`
- **Instruction and Data Storage**: Stored in linked lists for dynamic handling.
- **Error Handling**: Catches syntax errors, undefined labels, and invalid opcodes.

## 📂 File Structure
├── src/ # Source code directory │ ├── assembler.c # Main assembler file │ ├── preassembler.c # Pre-processing for macros │ ├── parser.c # Parses assembly instructions │ ├── symbol_table.c # Handles labels and addresses │ ├── error.c # Error handling functions │ ├── datastructures.c # Linked list implementation for storage │ └── globals.h # Global definitions ├── examples/ # Example assembly files ├── README.md # This file └── Makefile # Compilation automation


## 🚀 Installation & Compilation
1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/c-assembler.git
   cd c-assembler
2. Compile the assembelr
     make
3. ./assembler x y z (assuming x.asm,y.asm,z.asm)
