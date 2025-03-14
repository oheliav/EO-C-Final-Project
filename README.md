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
EO-C-Final-Project/
├── src/                # Source code directory
│   ├── assembler.c      # Main assembler file
│   ├── preassembler.c   # Pre-processing for macros
│   ├── parser.c         # Parses assembly instructions
│   ├── symbol_table.c   # Handles labels and addresses
│   ├── error.c          # Error handling functions
│   ├── datastructures.c # Linked list implementation for storage
│   ├── globals.h        # Global definitions
│   ├── instructions.c   # Handles instruction encoding
│   ├── opcodes.c        # Manages opcode lookups
│   └── utils.c          # Helper utility functions
├── include/             # Header files directory
│   ├── assembler.h      # Assembler header file
│   ├── preassembler.h   # Preassembler header file
│   ├── parser.h         # Parser header file
│   ├── symbol_table.h   # Symbol table header file
│   ├── error.h          # Error handling header file
│   ├── datastructures.h # Data structures header file
│   ├── instructions.h   # Instruction handling header file
│   ├── opcodes.h        # Opcodes lookup header file
│   └── utils.h          # Utility functions header file
├── examples/            # Example assembly files
│   ├── example1.as      # Example assembly file
│   ├── example2.as      # Another example
│   └── test_cases/      # Folder for test cases
├── output/              # Directory for generated machine code files
│   ├── example1.ob      # Object code output
│   ├── example1.ent     # Entry symbols file
│   ├── example1.ext     # External symbols file
│   ├── example2.ob      # Another object code output
│   ├── logs/            # Folder for error/debug logs
│   └── temp/            # Temporary files if needed
├── docs/                # Documentation (optional)
│   ├── README.md        # Project documentation
│   ├── DESIGN.md        # Design documentation
│   ├── INSTALL.md       # Installation guide
│   ├── USAGE.md         # Usage guide
│   └── ROADMAP.md       # Future plans
├── tests/               # Unit tests directory
│   ├── test_assembler.c # Tests for assembler
│   ├── test_parser.c    # Tests for parser
│   ├── test_symbol_table.c # Tests for symbol table
│   ├── test_opcodes.c   # Tests for opcodes
│   └── test_data.c      # Tests for data storage
├── .gitignore           # Ignore unnecessary files (binaries, temp files)
├── LICENSE              # Project license
├── Makefile             # Compilation automation
└── README.md            # Project overview



## 🚀 Installation & Compilation
1. Clone this repository:
   ```sh
   git clone https://github.com/oheliav/EO-C-Final-Project.git
   cd EO-C-Final-Project
2. Compile the assembelr
   ```make
3. Test it on your files
   ```./assembler x y z (assuming x.asm,y.asm,z.asm)
