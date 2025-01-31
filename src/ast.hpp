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
