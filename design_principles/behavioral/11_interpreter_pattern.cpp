/**
 * INTERPRETER PATTERN
 *
 * Intent: Define a representation for a grammar of a language and an interpreter
 * to interpret sentences in that language.
 *
 * Problem: Need to interpret/parse expressions or simple language
 * Solution: Define grammar as classes; interpreter evaluates expressions
 *
 * Real-world analogy: Interpreting SQL queries or mathematical expressions
 *
 * SOLID relation:
 * - SRP: Each expression type has single responsibility
 * - OCP: Add new expression types without modifying existing
 */

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <cctype>

// ============================================================================
// EXAMPLE 1: Mathematical Expression Interpreter
// ============================================================================

// PROBLEM: Need to parse and evaluate mathematical expressions
// ---
// Grammar:
// Expression = Term (('+' | '-') Term)*
// Term = Factor (('*' | '/') Factor)*
// Factor = Number | '(' Expression ')'
// Number = [0-9]+
//
// SOLUTION: Create expression classes, parser builds AST

// Context for variable values
class Context
{
private:
    std::unordered_map<std::string, int> variables_;

public:
    void setVariable(const std::string &name, int value)
    {
        variables_[name] = value;
    }

    int getVariable(const std::string &name) const
    {
        auto it = variables_.find(name);
        if (it != variables_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Variable not found: " + name);
    }
};

// Abstract expression
class Expression
{
public:
    virtual ~Expression() = default;
    virtual int interpret(const Context &context) const = 0;
};

// Terminal expression - number
class NumberExpression : public Expression
{
private:
    int number_;

public:
    NumberExpression(int num) : number_(num) {}
    int interpret(const Context &context) const override
    {
        return number_;
    }
};

// Terminal expression - variable
class VariableExpression : public Expression
{
private:
    std::string name_;

public:
    VariableExpression(const std::string &name) : name_(name) {}
    int interpret(const Context &context) const override
    {
        return context.getVariable(name_);
    }
};

// Non-terminal expression - addition
class AdditionExpression : public Expression
{
private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;

public:
    AdditionExpression(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left_(l), right_(r) {}

    int interpret(const Context &context) const override
    {
        return left_->interpret(context) + right_->interpret(context);
    }
};

// Non-terminal expression - subtraction
class SubtractionExpression : public Expression
{
private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;

public:
    SubtractionExpression(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left_(l), right_(r) {}

    int interpret(const Context &context) const override
    {
        return left_->interpret(context) - right_->interpret(context);
    }
};

// Non-terminal expression - multiplication
class MultiplicationExpression : public Expression
{
private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;

public:
    MultiplicationExpression(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left_(l), right_(r) {}

    int interpret(const Context &context) const override
    {
        return left_->interpret(context) * right_->interpret(context);
    }
};

// ============================================================================
// EXAMPLE 2: Boolean Expression Interpreter
// ============================================================================

// PROBLEM: Evaluate boolean conditions
// ---
// Grammar:
// BooleanExpression = BooleanTerm (('AND' | 'OR') BooleanTerm)*
// BooleanTerm = Variable | '(' BooleanExpression ')'

class BooleanContext
{
private:
    std::unordered_map<std::string, bool> conditions_;

public:
    void setCondition(const std::string &name, bool value)
    {
        conditions_[name] = value;
    }

    bool getCondition(const std::string &name) const
    {
        auto it = conditions_.find(name);
        if (it != conditions_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Condition not found: " + name);
    }
};

class BooleanExpression
{
public:
    virtual ~BooleanExpression() = default;
    virtual bool interpret(const BooleanContext &context) const = 0;
};

class BooleanVariable : public BooleanExpression
{
private:
    std::string name_;

public:
    BooleanVariable(const std::string &name) : name_(name) {}

    bool interpret(const BooleanContext &context) const override
    {
        return context.getCondition(name_);
    }
};

class AndExpression : public BooleanExpression
{
private:
    std::shared_ptr<BooleanExpression> left_;
    std::shared_ptr<BooleanExpression> right_;

public:
    AndExpression(std::shared_ptr<BooleanExpression> l, std::shared_ptr<BooleanExpression> r)
        : left_(l), right_(r) {}

    bool interpret(const BooleanContext &context) const override
    {
        return left_->interpret(context) && right_->interpret(context);
    }
};

class OrExpression : public BooleanExpression
{
private:
    std::shared_ptr<BooleanExpression> left_;
    std::shared_ptr<BooleanExpression> right_;

public:
    OrExpression(std::shared_ptr<BooleanExpression> l, std::shared_ptr<BooleanExpression> r)
        : left_(l), right_(r) {}

    bool interpret(const BooleanContext &context) const override
    {
        return left_->interpret(context) || right_->interpret(context);
    }
};

class NotExpression : public BooleanExpression
{
private:
    std::shared_ptr<BooleanExpression> expr_;

public:
    NotExpression(std::shared_ptr<BooleanExpression> e) : expr_(e) {}

    bool interpret(const BooleanContext &context) const override
    {
        return !expr_->interpret(context);
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== INTERPRETER PATTERN DEMO ===\n";

    // Arithmetic expression interpretation
    std::cout << "\n--- Mathematical Expression Interpreter ---\n";
    {
        Context context;
        context.setVariable("x", 10);
        context.setVariable("y", 5);

        // Build expression tree: 10 + (5 * 2)
        std::shared_ptr<Expression> expr1 = std::make_shared<NumberExpression>(10);
        std::shared_ptr<Expression> expr2 = std::make_shared<NumberExpression>(5);
        std::shared_ptr<Expression> expr3 = std::make_shared<NumberExpression>(2);

        std::shared_ptr<Expression> multiply =
            std::make_shared<MultiplicationExpression>(expr2, expr3);
        std::shared_ptr<Expression> addition =
            std::make_shared<AdditionExpression>(expr1, multiply);

        std::cout << "Expression: 10 + (5 * 2)\n";
        std::cout << "Result: " << addition->interpret(context) << "\n";

        // Build expression tree: x + y = 10 + 5
        std::shared_ptr<Expression> varX = std::make_shared<VariableExpression>("x");
        std::shared_ptr<Expression> varY = std::make_shared<VariableExpression>("y");
        std::shared_ptr<Expression> addition2 =
            std::make_shared<AdditionExpression>(varX, varY);

        std::cout << "\nExpression: x + y (where x=10, y=5)\n";
        std::cout << "Result: " << addition2->interpret(context) << "\n";

        // Build: x - y
        std::shared_ptr<Expression> subtraction =
            std::make_shared<SubtractionExpression>(varX, varY);
        std::cout << "\nExpression: x - y\n";
        std::cout << "Result: " << subtraction->interpret(context) << "\n";
    }

    // Boolean expression interpretation
    std::cout << "\n--- Boolean Expression Interpreter ---\n";
    {
        BooleanContext context;
        context.setCondition("isAdmin", true);
        context.setCondition("isActive", true);
        context.setCondition("isLocked", false);

        // Expression: isAdmin AND isActive
        std::shared_ptr<BooleanExpression> admin =
            std::make_shared<BooleanVariable>("isAdmin");
        std::shared_ptr<BooleanExpression> active =
            std::make_shared<BooleanVariable>("isActive");
        std::shared_ptr<BooleanExpression> canAccess =
            std::make_shared<AndExpression>(admin, active);

        std::cout << "Expression: isAdmin AND isActive\n";
        std::cout << "Result: " << (canAccess->interpret(context) ? "true" : "false") << "\n";

        // Expression: isAdmin OR isActive
        std::shared_ptr<BooleanExpression> orExpr =
            std::make_shared<OrExpression>(admin, active);
        std::cout << "\nExpression: isAdmin OR isActive\n";
        std::cout << "Result: " << (orExpr->interpret(context) ? "true" : "false") << "\n";

        // Expression: NOT isLocked
        std::shared_ptr<BooleanExpression> locked =
            std::make_shared<BooleanVariable>("isLocked");
        std::shared_ptr<BooleanExpression> notLocked =
            std::make_shared<NotExpression>(locked);
        std::cout << "\nExpression: NOT isLocked\n";
        std::cout << "Result: " << (notLocked->interpret(context) ? "true" : "false") << "\n";

        // Expression: (isAdmin AND isActive) AND (NOT isLocked)
        std::shared_ptr<BooleanExpression> finalExpr =
            std::make_shared<AndExpression>(canAccess, notLocked);
        std::cout << "\nExpression: (isAdmin AND isActive) AND (NOT isLocked)\n";
        std::cout << "Result: " << (finalExpr->interpret(context) ? "true" : "false") << "\n";
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Define grammar as class hierarchy\n";
    std::cout << "2. Terminal expressions for basic elements\n";
    std::cout << "3. Non-terminal expressions for combinations\n";
    std::cout << "4. AST (Abstract Syntax Tree) built from expressions\n";
    std::cout << "5. Context holds variable/condition values\n";
    std::cout << "6. interpret() evaluates the expression\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Need to interpret/parse a language
 * - Grammar is relatively simple
 * - Performance not critical (recursive interpretation can be slow)
 * - Want to extend language easily
 * - SQL, regex, expression parsers
 * - Configuration files
 * - Query languages
 *
 * BENEFITS:
 * - Easy to change and extend grammar
 * - Grammar represented as classes
 * - Uses abstract syntax tree (AST)
 * - Easy to interpret different languages
 * - Flexible representation
 *
 * DRAWBACKS:
 * - Can become complex for large grammars
 * - Performance overhead (recursive interpretation)
 * - Parser complexity not addressed
 * - Memory for large ASTs
 *
 * VARIATIONS:
 * - Parser generation (Lex, Yacc)
 * - AST optimization
 * - Bytecode compilation
 * - Visitor pattern for traversal
 *
 * COMPARISON:
 * - Interpreter: Parse and interpret language
 * - Composite: Tree structure
 * - Visitor: Operations on structure
 *
 * REAL WORLD:
 * - SQL parsers
 * - Expression evaluators
 * - Configuration file parsers
 * - Regular expressions
 * - DSLs (Domain Specific Languages)
 */
