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
