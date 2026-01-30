/**
 * TEMPLATE METHOD PATTERN
 *
 * Intent: Define the skeleton of an algorithm in a method, deferring some steps
 * to subclasses. Lets subclasses redefine certain steps without changing
 * the algorithm's structure.
 *
 * Problem: Multiple algorithms with common structure but different details
 * Solution: Define template method with common steps; subclasses override specific steps
 *
 * Real-world analogy: Recipe for cake - common steps (mix, bake, cool) but different ingredients
 *
 * SOLID relation:
 * - SRP: Base class defines structure, subclasses implement steps
 * - OCP: Add new implementations without modifying template
 * - LSP: Subclasses correctly implement template method
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Data Processing Pipeline
// ============================================================================

// PROBLEM: Different data sources need similar processing
// ---
// CSV, JSON, XML processing:
// 1. Read data
// 2. Parse data
// 3. Validate data
// 4. Process data
// 5. Output results
//
// Without Template Method:
// ✗ Duplicate processing logic in each parser
// ✗ Common steps repeated
// ✗ Hard to maintain - change one place, must change all
// ✗ Easy to make mistakes - miss a step
// ✗ Violates DRY principle
//
// Example messy WITHOUT Template Method:
//   void processCSV() {
//       readCSV();
//       parseCSV();
//       validateCSV();
//       process();
//       output();
//   }
//   void processJSON() {
//       readJSON();
//       parseJSON();
//       validateJSON();
//       process();      // duplicated!
//       output();       // duplicated!
//   }

// SOLUTION: Template Method - Define algorithm structure
// ---
// Key insight: Base class defines overall algorithm structure
// Subclasses override specific steps
//
// Benefits:
// ✓ Eliminates code duplication
// ✓ Algorithm structure clear in base class
// ✓ Easy to add new variations (OCP)
// ✓ Ensures steps executed in correct order
// ✓ Forced to implement required steps (abstract methods)
// ✓ Common logic in one place

// Template method pattern
class DataProcessor
{
public:
    virtual ~DataProcessor() = default;

    // Template method - defines algorithm structure
    void processData()
    {
        readData();
        parseData();
        validateData();
        transformData();
        outputResults();
    }

protected:
    // Common operations
    void readData()
    {
        std::cout << "  [Base] Reading data file\n";
    }

    void validateData()
    {
        std::cout << "  [Base] Validating data\n";
    }

    void outputResults()
    {
        std::cout << "  [Base] Output results\n";
    }

    // Hook methods - optional overrides
    virtual void beforeProcessing()
    {
        std::cout << "  [Base] No preprocessing\n";
    }

    // Abstract methods - must be implemented by subclasses
    virtual void parseData() = 0;
    virtual void transformData() = 0;
};

// Concrete implementations
class CSVProcessor : public DataProcessor
{
protected:
    void parseData() override
    {
        std::cout << "  [CSV] Parsing CSV format (comma-separated)\n";
    }

    void transformData() override
    {
        std::cout << "  [CSV] Converting CSV to internal format\n";
    }

    void beforeProcessing() override
    {
        std::cout << "  [CSV] Preprocessing: checking for BOM marker\n";
    }
};

class JSONProcessor : public DataProcessor
{
protected:
    void parseData() override
    {
        std::cout << "  [JSON] Parsing JSON format (object notation)\n";
    }

    void transformData() override
    {
        std::cout << "  [JSON] Converting JSON to internal format\n";
    }
};

class XMLProcessor : public DataProcessor
{
protected:
    void parseData() override
    {
        std::cout << "  [XML] Parsing XML format (tag-based)\n";
    }

    void transformData() override
    {
        std::cout << "  [XML] Converting XML to internal format\n";
    }

    void beforeProcessing() override
    {
        std::cout << "  [XML] Preprocessing: validating DTD\n";
    }
};

// ============================================================================
// EXAMPLE 2: Report Generation
// ============================================================================

// PROBLEM: Different report formats need same structure
// ---
// HTML, PDF, Text reports all need:
// - Title
// - Date
// - Data content
// - Footer with page number

class Report
{
public:
    virtual ~Report() = default;

    void generate()
    {
        printHeader();
        printContent();
        printFooter();
    }

protected:
    virtual void printHeader() = 0;
    virtual void printContent() = 0;
    virtual void printFooter() = 0;
};

class HTMLReport : public Report
{
protected:
    void printHeader() override
    {
        std::cout << "  <!DOCTYPE html>\n";
        std::cout << "  <html><head><title>Report</title></head>\n";
        std::cout << "  <body>\n";
    }

    void printContent() override
    {
        std::cout << "  <h1>Sales Report</h1>\n";
        std::cout << "  <p>Q1 Sales: $100,000</p>\n";
    }

    void printFooter() override
    {
        std::cout << "  </body></html>\n";
    }
};

class PDFReport : public Report
{
protected:
    void printHeader() override
    {
        std::cout << "  %PDF-1.4\n";
        std::cout << "  << /Type /Catalog >>\n";
    }

    void printContent() override
    {
        std::cout << "  (Sales Report)\n";
        std::cout << "  (Q1 Sales: $100,000)\n";
    }

    void printFooter() override
    {
        std::cout << "  xref\n";
        std::cout << "  trailer << /Size 5 >>\n";
    }
};

class PlainTextReport : public Report
{
protected:
    void printHeader() override
    {
        std::cout << "  ===== SALES REPORT =====\n";
        std::cout << "  Date: 2024\n\n";
    }

    void printContent() override
    {
        std::cout << "  Q1 Sales: $100,000\n";
        std::cout << "  Q2 Sales: $120,000\n";
    }

    void printFooter() override
    {
        std::cout << "  ===== END OF REPORT =====\n";
    }
};

// ============================================================================
// EXAMPLE 3: Game Character AI
// ============================================================================

// PROBLEM: Different enemy types have similar AI behavior
// ---
// Turn-based combat AI:
// 1. Analyze situation
// 2. Calculate best move
// 3. Execute move
// 4. Update game state
//
// But move calculation differs per enemy type

class Character
{
public:
    virtual ~Character() = default;

    void takeTurn()
    {
        std::cout << "  Taking turn:\n";
        analyzeSituation();
        calculateMove();
        executeMove();
        updateState();
    }

protected:
    virtual void analyzeSituation()
    {
        std::cout << "    - Analyzing game state\n";
    }

    virtual void updateState()
    {
        std::cout << "    - Updating character state\n";
    }

    virtual void calculateMove() = 0;
    virtual void executeMove() = 0;
};

class Goblin : public Character
{
protected:
    void calculateMove() override
    {
        std::cout << "    - Calculating move: Random attack\n";
    }

    void executeMove() override
    {
        std::cout << "    - Goblin attacks randomly!\n";
    }
};

class Wizard : public Character
{
protected:
    void analyzeSituation() override
    {
        std::cout << "    - Analyzing: Check mana, enemy HP\n";
    }

    void calculateMove() override
    {
        std::cout << "    - Calculating move: Cast spell based on mana\n";
    }

    void executeMove() override
    {
        std::cout << "    - Wizard casts Fireball!\n";
    }
};

class Dragon : public Character
{
protected:
    void analyzeSituation() override
    {
        std::cout << "    - Analyzing: Assess multiple enemies\n";
    }

    void calculateMove() override
    {
        std::cout << "    - Calculating move: AOE (area of effect) attack\n";
    }

    void executeMove() override
    {
        std::cout << "    - Dragon breathes fire on all enemies!\n";
    }

    void updateState() override
    {
        std::cout << "    - Recovering from fire breath...\n";
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== TEMPLATE METHOD PATTERN DEMO ===\n";

    // Data processing
    std::cout << "\n--- Data Processing Pipeline ---\n";
    {
        std::cout << "Processing CSV file:\n";
        CSVProcessor csv;
        csv.processData();

        std::cout << "\nProcessing JSON file:\n";
        JSONProcessor json;
        json.processData();

        std::cout << "\nProcessing XML file:\n";
        XMLProcessor xml;
        xml.processData();
    }

    // Report generation
    std::cout << "\n--- Report Generation ---\n";
    {
        std::cout << "Generating HTML Report:\n";
        HTMLReport htmlReport;
        htmlReport.generate();

        std::cout << "\nGenerating PDF Report:\n";
        PDFReport pdfReport;
        pdfReport.generate();

        std::cout << "\nGenerating Plain Text Report:\n";
        PlainTextReport textReport;
        textReport.generate();
    }

    // Game AI
    std::cout << "\n--- Game Character AI ---\n";
    {
        std::cout << "Goblin's turn:\n";
        Goblin goblin;
        goblin.takeTurn();

        std::cout << "\nWizard's turn:\n";
        Wizard wizard;
        wizard.takeTurn();

        std::cout << "\nDragon's turn:\n";
        Dragon dragon;
        dragon.takeTurn();
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Base class defines algorithm structure\n";
    std::cout << "2. Subclasses implement specific steps\n";
    std::cout << "3. Common code in one place\n";
    std::cout << "4. Forces correct step sequence\n";
    std::cout << "5. Easy to add variations\n";
    std::cout << "6. Hook methods provide extension points\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Multiple classes with similar algorithm structure
 * - Common parts duplicated
 * - Variations in only specific steps
 * - Want to ensure steps executed in order
 * - Want to control which steps subclasses can override
 * - Need to factor out common code
 *
 * BENEFITS:
 * - Eliminates code duplication (DRY)
 * - Algorithm structure clear in base class
 * - Easy to add variations (OCP)
 * - Ensures correct step sequence
 * - Hook methods allow optional customization
 * - Forced implementation (abstract methods)
 *
 * DRAWBACKS:
 * - Some subclasses may not need all steps
 * - Calling virtual methods (overhead)
 * - Inverse of normal call flow (Hollywood principle)
 * - Hard to refactor template method
 *
 * VARIATIONS:
 * - Hook methods (optional overrides)
 * - Final steps (can't override)
 * - Strategy combined with Template Method
 *
 * COMPARISON:
 * - Template Method: Base class defines structure
 * - Strategy: Client chooses algorithm
 * - Bridge: Separate abstraction from implementation
 *
 * RELATED CONCEPTS:
 * - Hollywood Principle: "Don't call us, we'll call you"
 * - Inversion of Control (IoC)
 * - Callback methods
 */
