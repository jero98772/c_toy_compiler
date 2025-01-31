Here's a simplified README for your C toy compiler project:
# c_toy_compiler..... HELPMEEEE I'M SUFERING

This project is an experiment in creating a simple toy compiler using LLVM. It compiles a basic Abstract Syntax Tree (AST) into LLVM Intermediate Representation (IR) and executes it using Just-In-Time (JIT) compilation.

## Prerequisites

Make sure you have the following installed:

- CMake
- LLVM (version 12 or later)
- A C++ compiler (e.g., `g++`)

## Build and Run

To compile and run the toy compiler:

```bash
# Clean previous builds (if any)
rm -rf build

# Create a new build directory and navigate into it
mkdir build && cd build

# Run CMake to configure the project
cmake ..

# Build the project
make

# Run the toy compiler
./toy_compiler
```

## How It Works

1. **AST Parsing**: The compiler parses a basic Abstract Syntax Tree (AST) to generate LLVM IR code.
2. **LLVM IR Generation**: It translates the AST into LLVM Intermediate Representation (IR).
3. **JIT Execution**: The compiler uses LLVM’s Just-In-Time (JIT) compilation to execute the generated IR.

## File Structure

- `codegen.cpp` - Contains the LLVM code generation logic.
- `codegen.hpp` - Header file for the `CodeGen` class.
- `main.cpp` - Main driver to run the compiler.
- `CMakeLists.txt` - Build configuration file.

## License

This project is for educational and experimental purposes only.

