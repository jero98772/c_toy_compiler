#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include <iostream>

/**
 * @brief Entry point for the program.
 * 
 * This is the main function that drives the process of lexical analysis, parsing,
 * code generation, and printing intermediate representation (IR).
 * It takes a simple arithmetic expression as a source string, lexes it, parses it
 * into an abstract syntax tree (AST), and then generates and prints the intermediate code.
 * 
 * The source string used here is "5 + 3;" for testing purposes.
 * 
 * @return Exit status of the program.
 */
int main() {
    // The source code to be processed (simple arithmetic expression).
    std::string source = "5 + 3;";

    // Create a lexer object with the given source code.
    // The lexer tokenizes the input string into meaningful components (tokens).
    Lexer lexer(source);

    // Create a parser object, passing the lexer to it.
    // The parser processes the tokens from the lexer and builds an abstract syntax tree (AST).
    Parser parser(lexer);

    // Parse the expression into an AST.
    // The AST represents the structure of the expression.
    auto ast = parser.parseExpression();

    // Create a code generator object.
    // This will generate intermediate representation (IR) from the AST.
    CodeGen codeGen;

    // Generate the intermediate code from the AST.
    codeGen.generate(ast.get());

    // Print the generated intermediate representation.
    codeGen.printIR();

    // Return 0 to indicate successful execution.
    return 0;
}
