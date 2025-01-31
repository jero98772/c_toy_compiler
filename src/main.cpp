#include <fstream>
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

/**
 * @brief The main entry point for the compiler program.
 * 
 * This program takes a source file as input, tokenizes it, parses it into an abstract 
 * syntax tree (AST), generates intermediate representation (IR) code, and optionally 
 * executes the IR code using JIT compilation.
 * 
 * Usage: 
 * ./toy_compiler <source-file>
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the program completes successfully, or 1 if there was an error.
 */
int main(int argc, char* argv[]) {
    // Check if the source file argument is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source-file>\n";
        return 1;
    }

    // Open the source file for reading
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // Read the entire source file into a string
    std::string source((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Create a lexer and parser for tokenizing and parsing the source code
    Lexer lexer(source);
    Parser parser(lexer);

    // Parse the source code into an abstract syntax tree (AST)
    auto ast = parser.parseExpression();

    // Generate the intermediate representation (IR) code from the AST
    CodeGen codeGen;
    codeGen.generate(ast.get());

    // Print the generated IR code
    codeGen.printIR();

    // Uncomment the next line to run JIT execution of the generated IR
    // codeGen.runJIT();

    return 0;
}
