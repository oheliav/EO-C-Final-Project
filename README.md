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
```EO-C-Final-Project/
├── LICENSE              # Project license
├── README.md            # Project documentation
├── bitUtils.c           # Bitwise utilities implementation
├── bitUtils.h           # Bitwise utilities header
├── dataStructures.c     # Data structures implementation
├── dataStructures.h     # Data structures header
├── errors.c             # Error handling implementation
├── errors.h             # Error handling header
├── firstPass.c          # First pass of the assembler
├── firstPass.h          # First pass header file
├── globals.c            # Global variables implementation
├── globals.h            # Global variables header
├── main.c               # Main assembler program
├── main.h               # Main assembler header
├── makefile             # Compilation automation
├── preAssembler.c       # Pre-assembler implementation
├── preAssembler.h       # Pre-assembler header
├── run.bat              # Windows batch script to run the assembler
├── secondPass.c         # Second pass of the assembler
├── secondPass.h         # Second pass header file
├── util.c               # Utility functions implementation
├── util.h               # Utility functions header




## 🚀 Installation & Compilation
1. Clone this repository:
   ```sh
   git clone https://github.com/oheliav/EO-C-Final-Project.git
   cd EO-C-Final-Project
2. Compile the assembelr
   ```make
3. Test it on your files
   ```./assembler x y z (assuming x.asm,y.asm,z.asm)
