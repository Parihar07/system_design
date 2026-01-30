/**
 * VISITOR PATTERN
 *
 * Intent: Represent an operation to be performed on elements of an object structure.
 * Visitor lets you define a new operation without changing the classes of the elements.
 *
 * Problem: Need to perform operations on complex object structures without modifying classes
 * Solution: Encapsulate operation in separate visitor class
 *
 * Real-world analogy: Tax advisor visiting different people - each person accepts advisor
 *
 * SOLID relation:
 * - SRP: Visitor handles specific operation, separate from element classes
 * - OCP: Add new operations without modifying element classes
 * - DIP: Elements depend on Visitor abstraction
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// EXAMPLE 1: Tax Calculation on Different Income Types
// ============================================================================

// PROBLEM: Different income types have different tax calculations
// ---
// Income types: Salary, Investment, Business
// Operations: Calculate Tax, Calculate Deductions, Generate Report
//
// Without Visitor:
// ✗ Each income type must know about all calculations
// ✗ Adding new operation requires modifying all income classes
// ✗ Tax logic scattered across multiple classes
// ✗ Hard to test calculations independently
// ✗ Violates SRP and OCP
//
// Example messy WITHOUT Visitor:
//   class Salary {
//       calculateTax() { ... }
//       calculateDeductions() { ... }
//       generateReport() { ... }
//   }
//   class Investment {
//       calculateTax() { ... }
//       calculateDeductions() { ... }
//       generateReport() { ... }
//   }
//   // Duplicate logic everywhere!

// SOLUTION: Visitor - Encapsulate operations
// ---
// Key insight: Operations visit elements of structure
// 1. Define Visitor interface with visit methods
// 2. Each element implements accept(visitor)
// 3. Add new operations by creating new visitor
// 4. Elements unchanged when adding operations
//
// Benefits:
// ✓ Operations separated from element classes
// ✓ Easy to add new operations (OCP)
// ✓ Element classes don't need to know about operations
// ✓ Related operations grouped in visitor
// ✓ SRP - each visitor handles one operation
// ✓ Elements don't change when operations change

// Forward declarations
class IncomeVisitor;

class Income
{
public:
    virtual ~Income() = default;
    virtual double getAmount() const = 0;
    virtual void accept(IncomeVisitor &visitor) = 0;
};

class IncomeVisitor
{
public:
    virtual ~IncomeVisitor() = default;
    virtual void visitSalary(class Salary &salary) = 0;
    virtual void visitInvestment(class Investment &investment) = 0;
    virtual void visitBusiness(class Business &business) = 0;
};

// Concrete income types
class Salary : public Income
{
private:
    double amount_;

public:
    Salary(double amt) : amount_(amt) {}
    double getAmount() const override { return amount_; }
    void accept(IncomeVisitor &visitor) override;
};

class Investment : public Income
{
private:
    double dividend_;

public:
    Investment(double div) : dividend_(div) {}
    double getAmount() const override { return dividend_; }
    void accept(IncomeVisitor &visitor) override;
};

class Business : public Income
{
private:
    double profit_;

public:
    Business(double prof) : profit_(prof) {}
    double getAmount() const override { return profit_; }
    void accept(IncomeVisitor &visitor) override;
};

// Concrete visitors
class TaxCalculator : public IncomeVisitor
{
private:
    double totalTax_ = 0;

public:
    void visitSalary(Salary &salary) override
    {
        double tax = salary.getAmount() * 0.20; // 20% tax on salary
        totalTax_ += tax;
        std::cout << "  [TaxCalc] Salary tax: $" << tax << "\n";
    }

    void visitInvestment(Investment &investment) override
    {
        double tax = investment.getAmount() * 0.15; // 15% on dividends
        totalTax_ += tax;
        std::cout << "  [TaxCalc] Investment tax: $" << tax << "\n";
    }

    void visitBusiness(Business &business) override
    {
        double tax = business.getAmount() * 0.25; // 25% on business profit
        totalTax_ += tax;
        std::cout << "  [TaxCalc] Business tax: $" << tax << "\n";
    }

    double getTotalTax() const { return totalTax_; }
};

class DeductionCalculator : public IncomeVisitor
{
private:
    double totalDeductions_ = 0;

public:
    void visitSalary(Salary &salary) override
    {
        double deduction = salary.getAmount() * 0.05; // 5% deduction
        totalDeductions_ += deduction;
        std::cout << "  [Deduct] Salary deduction: $" << deduction << "\n";
    }

    void visitInvestment(Investment &investment) override
    {
        double deduction = investment.getAmount() * 0.03; // 3% deduction
        totalDeductions_ += deduction;
        std::cout << "  [Deduct] Investment deduction: $" << deduction << "\n";
    }

    void visitBusiness(Business &business) override
    {
        double deduction = business.getAmount() * 0.10; // 10% deduction
        totalDeductions_ += deduction;
        std::cout << "  [Deduct] Business deduction: $" << deduction << "\n";
    }

    double getTotalDeductions() const { return totalDeductions_; }
};

// Implement accept methods
void Salary::accept(IncomeVisitor &visitor)
{
    visitor.visitSalary(*this);
}

void Investment::accept(IncomeVisitor &visitor)
{
    visitor.visitInvestment(*this);
}

void Business::accept(IncomeVisitor &visitor)
{
    visitor.visitBusiness(*this);
}

// ============================================================================
// EXAMPLE 2: File System Operations
// ============================================================================

// PROBLEM: Need to perform operations on mixed file/directory structure
// ---
// Operations: Calculate size, Count files, Generate report

class FileSystemVisitor;

class FileSystemElement
{
public:
    virtual ~FileSystemElement() = default;
    virtual void accept(FileSystemVisitor &visitor) = 0;
};

class FileSystemVisitor
{
public:
    virtual ~FileSystemVisitor() = default;
    virtual void visitFile(class File &file) = 0;
    virtual void visitDirectory(class Directory &dir) = 0;
};

class File : public FileSystemElement
{
private:
    std::string name_;
    long size_;

public:
    File(const std::string &n, long s) : name_(n), size_(s) {}
    std::string getName() const { return name_; }
    long getSize() const { return size_; }
    void accept(FileSystemVisitor &visitor) override;
};

class Directory : public FileSystemElement
{
private:
    std::string name_;
    std::vector<std::shared_ptr<FileSystemElement>> elements_;

public:
    Directory(const std::string &n) : name_(n) {}
    std::string getName() const { return name_; }

    void add(std::shared_ptr<FileSystemElement> element)
    {
        elements_.push_back(element);
    }

    void accept(FileSystemVisitor &visitor) override;

    const std::vector<std::shared_ptr<FileSystemElement>> &getElements() const
    {
        return elements_;
    }
};

class SizeCalculator : public FileSystemVisitor
{
private:
    long totalSize_ = 0;
    int indentLevel_ = 0;

public:
    void visitFile(File &file) override
    {
        totalSize_ += file.getSize();
        for (int i = 0; i < indentLevel_; i++)
            std::cout << "  ";
        std::cout << file.getName() << " (" << file.getSize() << " bytes)\n";
    }

    void visitDirectory(Directory &dir) override
    {
        for (int i = 0; i < indentLevel_; i++)
            std::cout << "  ";
        std::cout << "[" << dir.getName() << "]\n";

        indentLevel_++;
        for (const auto &element : dir.getElements())
        {
            element->accept(*this);
        }
        indentLevel_--;
    }

    long getTotalSize() const { return totalSize_; }
};

void File::accept(FileSystemVisitor &visitor)
{
    visitor.visitFile(*this);
}

void Directory::accept(FileSystemVisitor &visitor)
{
    visitor.visitDirectory(*this);
}

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== VISITOR PATTERN DEMO ===\n";

    // Income tax calculation
    std::cout << "\n--- Tax Calculation with Visitor ---\n";
    {
        std::vector<std::shared_ptr<Income>> incomes;
        incomes.push_back(std::make_shared<Salary>(100000));
        incomes.push_back(std::make_shared<Investment>(50000));
        incomes.push_back(std::make_shared<Business>(200000));

        std::cout << "Calculating taxes:\n";
        TaxCalculator taxCalc;
        for (auto &income : incomes)
        {
            income->accept(taxCalc);
        }
        std::cout << "Total tax: $" << taxCalc.getTotalTax() << "\n";

        std::cout << "\nCalculating deductions:\n";
        DeductionCalculator deductCalc;
        for (auto &income : incomes)
        {
            income->accept(deductCalc);
        }
        std::cout << "Total deductions: $" << deductCalc.getTotalDeductions() << "\n";
    }

    // File system operations
    std::cout << "\n--- File System Structure ---\n";
    {
        auto root = std::make_shared<Directory>("root");
        auto documents = std::make_shared<Directory>("documents");
        auto images = std::make_shared<Directory>("images");

        documents->add(std::make_shared<File>("report.doc", 5000));
        documents->add(std::make_shared<File>("notes.txt", 2000));

        images->add(std::make_shared<File>("photo1.jpg", 2000000));
        images->add(std::make_shared<File>("photo2.jpg", 1800000));

        root->add(documents);
        root->add(images);
        root->add(std::make_shared<File>("readme.txt", 3000));

        std::cout << "File system structure and sizes:\n";
        SizeCalculator sizeCalc;
        root->accept(sizeCalc);
        std::cout << "Total size: " << sizeCalc.getTotalSize() << " bytes\n";
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Separate operations from object structure\n";
    std::cout << "2. Add new operations without modifying classes\n";
    std::cout << "3. Elements accept visitors\n";
    std::cout << "4. Visitor implements operation logic\n";
    std::cout << "5. Works with complex object hierarchies\n";
    std::cout << "6. Double dispatch pattern\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Need multiple operations on object structure
 * - Many unrelated operations on complex object structure
 * - Object classes rarely change, operations often change
 * - Want to avoid polluting classes with operations
 * - Operations need access to private data
 * - Want to perform operations across entire structure
 *
 * BENEFITS:
 * - Separates operations from object classes
 * - Easy to add new operations (OCP)
 * - Gathers related operations in visitor
 * - Object structure unchanged when operations change
 * - SRP - each visitor handles one operation
 * - Works with complex hierarchies
 *
 * DRAWBACKS:
 * - Hard to add new element types (must update all visitors)
 * - Breaks encapsulation (visitors need access)
 * - Double dispatch complexity
 * - Overkill for simple cases
 * - Hard to understand initially
 *
 * ADVANCED ASPECTS:
 * - Double dispatch: Runtime selection of method based on two objects
 * - Visitor hierarchy: Visitors can extend each other
 * - Composite+Visitor: Perfect for tree traversal
 *
 * COMPARISON:
 * - Visitor: Operation visits elements
 * - Composite: Recursive structure
 * - Interpreter: Define language grammar
 * - Strategy: Choose algorithm
 */
