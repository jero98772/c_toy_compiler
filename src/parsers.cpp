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
