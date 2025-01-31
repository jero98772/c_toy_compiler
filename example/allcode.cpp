lexer.hpp
#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

/**
 * @brief Enum representing the different types of tokens.
 * 
 * This enum class defines all the possible types of tokens that the lexer can
 * recognize in the source code.
 */
enum class TokenType {
    INT,         /**< Represents the 'int' keyword */
    RETURN,      /**< Represents the 'return' keyword */
    IF,          /**< Represents the 'if' keyword */
    ELSE,        /**< Represents the 'else' keyword */
    WHILE,       /**< Represents the 'while' keyword */
    IDENTIFIER,  /**< Represents an identifier (variable or function name) */
    NUMBER,      /**< Represents a number */
    OPERATOR,    /**< Represents an operator (+, -, *, /) */
    PAREN_OPEN,  /**< Represents an open parenthesis '(' */
    PAREN_CLOSE, /**< Represents a close parenthesis ')' */
    BRACE_OPEN,  /**< Represents an open brace '{' */
    BRACE_CLOSE, /**< Represents a close brace '}' */
    SEMICOLON,   /**< Represents a semicolon ';' */
    END          /**< Represents the end of the input */
};

/**
 * @brief Represents a single token.
 * 
 * A token consists of a type (from the TokenType enum) and its value (a string
 * representation of the token).
 */
struct Token {
    TokenType type;  /**< The type of the token */
    std::string value; /**< The value of the token as a string */
};

/**
 * @brief Lexer class for tokenizing source code.
 * 
 * The Lexer class processes the input source code and generates a sequence of
 * tokens that represent the building blocks of the code. The lexer handles keywords,
 * operators, identifiers, numbers, and other symbols.
 */
class Lexer {
public:
    /**
     * @brief Constructs a Lexer with the provided source code.
     * 
     * Initializes the lexer with the source code to tokenize. The lexer will process
     * the code character by character.
     * 
     * @param source The source code to tokenize.
     */
    explicit Lexer(const std::string &source);

    /**
     * @brief Retrieves the next token from the source code.
     * 
     * Scans the source code character by character, identifying and returning
     * the next token. If the end of the source code is reached, an END token is returned.
     * 
     * @return A Token representing the next token in the source code.
     */
    Token getNextToken();

private:
    std::string source; /**< The source code to tokenize */
    size_t pos = 0;     /**< The current position in the source code */
};

#endif


lexer.cpp

#include "lexer.hpp"
#include <cctype>

/**
 * @brief Constructs a Lexer object that processes the given source string.
 * 
 * The lexer will iterate through the provided source string character by character
 * and convert the input into a sequence of tokens.
 * 
 * @param src The source string to tokenize.
 */
Lexer::Lexer(const std::string &src) : source(src), pos(0) {}

/**
 * @brief Retrieves the next token from the source string.
 * 
 * The method processes the input string character by character, ignoring whitespace
 * and identifying numbers, keywords, operators, identifiers, and other symbols.
 * 
 * @return A Token object representing the next token in the source code.
 *         If the end of the source string is reached, returns a token of type END.
 */
Token Lexer::getNextToken() {
    // Skip any whitespace characters.
    while (pos < source.length() && isspace(source[pos])) pos++;

    // If we have reached the end of the source, return an END token.
    if (pos >= source.length()) return {TokenType::END, ""};

    char current = source[pos];

    // If the current character is a digit, it represents a number.
    if (isdigit(current)) {
        std::string num;
        while (isdigit(source[pos])) num += source[pos++];
        return {TokenType::NUMBER, num};
    }

    // If the current character is a letter, it may represent a keyword or identifier.
    if (isalpha(current)) {
        std::string ident;
        while (isalnum(source[pos])) ident += source[pos++];
        if (ident == "int") return {TokenType::INT, ident};
        if (ident == "return") return {TokenType::RETURN, ident};
        return {TokenType::IDENTIFIER, ident};
    }

    // Handle various operators and symbols.
    switch (current) {
        case '+': case '-': case '*': case '/': 
            return {TokenType::OPERATOR, std::string(1, source[pos++])};
        case '(': 
            return {TokenType::PAREN_OPEN, "("}; pos++;
        case ')': 
            return {TokenType::PAREN_CLOSE, ")"}; pos++;
        case '{': 
            return {TokenType::BRACE_OPEN, "{"}; pos++;
        case '}': 
            return {TokenType::BRACE_CLOSE, "}"}; pos++;
        case ';': 
            return {TokenType::SEMICOLON, ";"}; pos++;
    }

    // If no valid token is found, return END token.
    return {TokenType::END, ""};
}


main.cpp
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

parsers.h
#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "ast.hpp"

/**
 * @brief Parser class for parsing tokens into an abstract syntax tree (AST).
 * 
 * The Parser class processes a sequence of tokens provided by the Lexer and 
 * constructs an abstract syntax tree (AST). This AST represents the syntactical 
 * structure of the source code based on the grammar rules. The parser is responsible 
 * for handling expressions and constructing nodes for the tree.
 */
class Parser {
public:
    /**
     * @brief Constructs a Parser with the provided Lexer.
     * 
     * The constructor initializes the parser with the given Lexer object. The lexer 
     * will be used to obtain tokens for parsing. This sets up the initial state for 
     * parsing expressions from the token stream.
     * 
     * @param lexer The lexer that provides tokens to be parsed.
     */
    explicit Parser(Lexer &lexer);

    /**
     * @brief Parses an expression and returns the corresponding AST node.
     * 
     * This method processes the tokens from the lexer and constructs the abstract 
     * syntax tree (AST) for an expression. It handles the parsing logic and builds 
     * the appropriate AST nodes based on the syntax of the expression.
     * 
     * @return A unique pointer to the root AST node representing the parsed expression.
     */
    std::unique_ptr<ASTNode> parseExpression();

    /**
     * @brief Parses an "if" statement and returns the corresponding AST node.
     * 
     * This method processes the tokens from the lexer to parse an "if" statement 
     * in the source code and constructs an AST node that represents it. It is part 
     * of the grammar rules for handling conditional branching in the code.
     * 
     * @return A unique pointer to the root AST node representing the parsed "if" statement.
     */
    std::unique_ptr<ASTNode> parseIfStatement();

    /**
     * @brief Parses a "while" statement and returns the corresponding AST node.
     * 
     * This method processes the tokens from the lexer to parse a "while" loop statement 
     * in the source code and constructs an AST node that represents it. This handles 
     * the parsing logic for looping constructs.
     * 
     * @return A unique pointer to the root AST node representing the parsed "while" statement.
     */
    std::unique_ptr<ASTNode> parseWhileStatement();

private:
    Lexer &lexer;         /**< Reference to the lexer used for tokenizing the input */
    Token currentToken;   /**< The current token being processed */
};

#endif


parsers.cpp
#include "parser.hpp"

/**
 * @brief Constructs a Parser with the provided Lexer.
 * 
 * The constructor initializes the parser with a reference to the Lexer object.
 * It also retrieves the first token from the lexer to begin parsing.
 * 
 * @param lex The lexer object used for tokenizing the input source code.
 */
Parser::Parser(Lexer &lex) : lexer(lex) { currentToken = lexer.getNextToken(); }

/**
 * @brief Parses an expression and returns the corresponding AST node.
 * 
 * This method parses a simple arithmetic expression. It handles the parsing 
 * of numbers, operators, and constructs a binary expression tree if there is an 
 * operator in the expression. The method supports recursive parsing for expressions 
 * with multiple operands and operators.
 * 
 * @return A unique pointer to the root AST node representing the parsed expression.
 *         It can either be a single number or a binary expression.
 */
std::unique_ptr<ASTNode> Parser::parseExpression() {
    // Parse the left-hand side of the expression as a number.
    auto left = std::make_unique<NumberExpr>(std::stoi(currentToken.value));

    // Move to the next token after parsing the number.
    currentToken = lexer.getNextToken();

    // If the next token is an operator, parse the right-hand side of the expression.
    if (currentToken.type == TokenType::OPERATOR) {
        // Get the operator and move to the next token.
        char op = currentToken.value[0];
        currentToken = lexer.getNextToken();

        // Recursively parse the right-hand side expression and create a BinaryExpr node.
        auto right = parseExpression();
        return std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    // If no operator is found, return the left-hand side (which is a number).
    return left;
}

/**
 * @brief Parses an "if" statement and returns the corresponding AST node.
 * 
 * This method processes the tokens to parse an "if" statement, which consists 
 * of a condition, a "then" branch, and optionally an "else" branch. It constructs 
 * an IfStatement node that represents the structure of the conditional statement.
 * 
 * @return A unique pointer to the AST node representing the parsed "if" statement.
 */
std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    currentToken = lexer.getNextToken(); // Skip 'if'
    
    // Parse the condition expression inside the if statement.
    auto condition = parseExpression();
    
    currentToken = lexer.getNextToken(); // Skip '{'
    
    // Parse the then branch of the if statement.
    auto thenBranch = parseExpression();
    
    std::unique_ptr<ASTNode> elseBranch = nullptr;

    // If there is an 'else' part, parse it.
    if (currentToken.type == TokenType::ELSE) {
        currentToken = lexer.getNextToken();
        elseBranch = parseExpression();
    }

    // Return the constructed IfStatement node.
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

/**
 * @brief Parses a "while" statement and returns the corresponding AST node.
 * 
 * This method processes the tokens to parse a "while" loop, which consists of 
 * a condition and a body. It constructs a WhileStatement node representing 
 * the structure of the loop.
 * 
 * @return A unique pointer to the AST node representing the parsed "while" statement.
 */
std::unique_ptr<ASTNode> Parser::parseWhileStatement() {
    currentToken = lexer.getNextToken(); // Skip 'while'
    
    // Parse the condition expression inside the while loop.
    auto condition = parseExpression();
    
    currentToken = lexer.getNextToken(); // Skip '{'
    
    // Parse the body of the while loop.
    auto body = parseExpression();
    
    // Return the constructed WhileStatement node.
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}


codegen.hpp
#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "ast.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

/**
 * @class CodeGen
 * @brief A simple LLVM-based code generator for an AST.
 */
class CodeGen {
public:
    /**
     * @brief Constructs a CodeGen instance and initializes LLVM structures.
     */
    CodeGen();

    /**
     * @brief Generates LLVM IR for a given AST node.
     * 
     * This function takes an ASTNode and generates the corresponding LLVM IR representation.
     * 
     * @param node Pointer to the ASTNode to generate code for.
     * @return llvm::Value* The generated LLVM IR value.
     */
    llvm::Value* generate(ASTNode *node);

    void runJIT();
    /**
     * @brief Prints the generated LLVM IR to the standard output.
     */
    void printIR();

private:
    llvm::LLVMContext context; ///< The LLVM context for managing global state.
    llvm::Module module; ///< The LLVM module containing the generated code.
    llvm::IRBuilder<> builder; ///< The LLVM IR builder for creating instructions.
};

#endif

codegen.cpp
#include "codegen.hpp"
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

// ... Existing CodeGen methods ...

void CodeGen::runJIT() {
    // Initialize LLVM targets
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Create an ExecutorProcessControl
    auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
    if (!EPC) {
        std::cerr << "Failed to create ExecutorProcessControl: "
                  << llvm::toString(EPC.takeError()) << "\n";
        return;
    }

    // Create ExecutionSession
    llvm::orc::ExecutionSession execSession(std::move(*EPC));

    // Obtain the target triple from the current module
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    module.setTargetTriple(targetTriple);

    // Create JITTargetMachineBuilder
    llvm::orc::JITTargetMachineBuilder JTMB(targetTriple);

    // Create DataLayout
    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL) {
        std::cerr << "Failed to get DataLayout: " << llvm::toString(DL.takeError()) << "\n";
        return;
    }

    // Create IRCompileLayer with ConcurrentIRCompiler
    llvm::orc::IRCompileLayer compileLayer(
        execSession,
        llvm::orc::ConcurrentIRCompiler(std::move(JTMB)));

    // Create main JITDylib and add dynamic library search
    auto &mainJD = execSession.createBareJITDylib("main");
    mainJD.addGenerator(llvm::cantFail(
        llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            (*DL)->getGlobalPrefix())));

    // Clone the module to transfer ownership to the JIT
    std::unique_ptr<llvm::Module> clonedModule = llvm::CloneModule(module);
    llvm::orc::ThreadSafeModule TSM(
        std::move(clonedModule),
        std::make_unique<llvm::LLVMContext>());

    // Add the module to the JIT
    if (auto err = compileLayer.add(mainJD, std::move(TSM))) {
        std::cerr << "Failed to add module to JIT: " << llvm::toString(std::move(err)) << "\n";
        return;
    }

    // Look up the 'main' function
    auto mainSym = execSession.lookup({&mainJD}, "main");
    if (!mainSym) {
        std::cerr << "Failed to lookup 'main': " << llvm::toString(mainSym.takeError()) << "\n";
        return;
    }

    // Cast the symbol to a function pointer and execute
    auto *mainFunc = (void (*)())(mainSym->getAddress());
    mainFunc();

    // Shutdown the execution session
    if (auto err = execSession.endSession()) {
        std::cerr << "Error shutting down session: " << llvm::toString(std::move(err)) << "\n";
    }
}

ast.hpp

#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>
#include <string>

/**
 * @brief Base class for all Abstract Syntax Tree (AST) nodes.
 * 
 * This is the base class for all AST nodes. It provides a virtual destructor to ensure
 * proper cleanup of derived classes.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default; ///< Virtual destructor to ensure proper cleanup of derived classes.
};

/**
 * @brief Represents a numeric expression node in the AST.
 * 
 * This class represents a numeric expression, typically a constant integer value. It
 * stores the integer value and is used in arithmetic expressions.
 */
class NumberExpr : public ASTNode {
public:
    int value; ///< The integer value of the numeric expression.

    /**
     * @brief Constructs a NumberExpr with a given integer value.
     * 
     * @param val The integer value to be stored in this node.
     */
    explicit NumberExpr(int val) : value(val) {}
};

/**
 * @brief Represents a binary expression node in the AST (e.g., addition, subtraction).
 * 
 * This class represents binary operations such as addition, subtraction, multiplication, 
 * and division. It contains pointers to the left and right operands, as well as the operator.
 */
class BinaryExpr : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;  ///< Pointer to the left operand.
    std::unique_ptr<ASTNode> right; ///< Pointer to the right operand.
    char op; ///< The binary operator (e.g., '+', '-', '*', '/').

    /**
     * @brief Constructs a BinaryExpr with a left operand, an operator, and a right operand.
     * 
     * @param l Unique pointer to the left operand.
     * @param o The binary operator.
     * @param r Unique pointer to the right operand.
     */
    BinaryExpr(std::unique_ptr<ASTNode> l, char o, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
};

/**
 * @brief Represents an "if" statement node in the AST.
 * 
 * This class represents an "if" statement, consisting of a condition, a "then" branch,
 * and optionally an "else" branch. It constructs a node that holds these components.
 */
class IfStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition; ///< The condition to evaluate.
    std::unique_ptr<ASTNode> thenBranch; ///< The expression to execute if the condition is true.
    std::unique_ptr<ASTNode> elseBranch; ///< The expression to execute if the condition is false (optional).

    /**
     * @brief Constructs an IfStatement with a condition, then branch, and optional else branch.
     * 
     * @param cond The condition to evaluate.
     * @param thenBr The expression to execute if the condition is true.
     * @param elseBr The expression to execute if the condition is false (optional).
     */
    IfStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr, std::unique_ptr<ASTNode> elseBr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}
};

/**
 * @brief Represents a "while" statement node in the AST.
 * 
 * This class represents a "while" loop statement, consisting of a condition and a body. 
 * The loop will continue executing the body as long as the condition evaluates to true.
 */
class WhileStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition; ///< The condition to evaluate before each iteration.
    std::unique_ptr<ASTNode> body; ///< The body of the loop to execute.

    /**
     * @brief Constructs a WhileStatement with a condition and body.
     * 
     * @param cond The condition to evaluate before each iteration.
     * @param b The body of the loop to execute.
     */
    WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};

/**
 * @brief Represents a function call node in the AST.
 * 
 * This class represents a function call, storing the function name and the list of arguments 
 * passed to the function. Each argument is stored as a shared pointer to an AST node, 
 * allowing for a flexible representation of the arguments.
 */
class FunctionCall : public ASTNode {
public:
    std::string name; ///< The name of the function being called.
    std::vector<std::shared_ptr<ASTNode>> args; ///< The arguments passed to the function.

    /**
     * @brief Constructs a FunctionCall with a given function name.
     * 
     * @param name The name of the function being called.
     */
    FunctionCall(const std::string& name) : name(name) {}
};

#endif // AST_HPP
