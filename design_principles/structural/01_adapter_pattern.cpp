/**
 * ADAPTER PATTERN
 *
 * Intent: Convert the interface of a class into another interface clients expect.
 * Lets classes work together that couldn't otherwise because of incompatible interfaces.
 *
 * Problem: You have existing code that expects interface A, but you have a class with interface B.
 * Solution: Create an adapter that wraps B and presents interface A.
 *
 * Real-world analogy: Power adapter for travel - US plug to EU socket
 *
 * SOLID relation:
 * - OCP: Extend functionality without modifying existing code
 * - ISP: Client depends on abstract interface, not concrete implementation
 * - DIP: Depend on abstractions (target interface), not concretions
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Payment Gateway Integration
// ============================================================================

// PROBLEM: Incompatible interfaces - What's wrong?
// ---
// You bought a new payment processing library, but it has a different interface
// than what your application expects. The library uses:
//   - processPaymentOldWay(account, amount)
//   - verifyAccountOldWay(account)
//
// But your application expects:
//   - authorize(userId, amount)
//   - charge(userId, amount)
//
// Options WITHOUT adapter:
// ✗ Option 1: Modify the library - NOT POSSIBLE (third-party, closed source)
// ✗ Option 2: Modify all client code - MESSY, error-prone, maintenance nightmare
// ✗ Option 3: Duplicate logic everywhere - VIOLATES DRY, causes bugs
// ✗ Option 4: Hard-code translation scattered throughout - TIGHT COUPLING
//
// Result: Business logic polluted with translation code everywhere!

// Legacy payment system (third-party - can't modify)
class LegacyPaymentProcessor
{
public:
    void processPaymentOldWay(const std::string &account, double amount)
    {
        std::cout << "[Legacy] Processing $" << amount
                  << " from account: " << account << "\n";
    }

    void verifyAccountOldWay(const std::string &account)
    {
        std::cout << "[Legacy] Verifying account: " << account << "\n";
    }
};

// New system interface expected by the application
class IPaymentGateway
{
public:
    virtual ~IPaymentGateway() = default;
    virtual bool authorize(const std::string &userId, double amount) = 0;
    virtual bool charge(const std::string &userId, double amount) = 0;
};

// SOLUTION: Object Adapter (composition - preferred)
// ---
// Create an adapter that:
// 1. Implements the interface YOUR CODE expects (IPaymentGateway)
// 2. Wraps the incompatible library (LegacyPaymentProcessor)
// 3. Translates calls transparently
//
// Benefits:
// ✓ Keeps business logic clean - no translation scattered everywhere
// ✓ One place to modify if library interface changes
// ✓ Client code doesn't know about legacy system
// ✓ Can swap adapters for different libraries
// ✓ Follows SRP - adapter handles translation only
// ✓ Follows OCP - extend without modifying existing code

class LegacyPaymentAdapter : public IPaymentGateway
{
private:
    std::unique_ptr<LegacyPaymentProcessor> legacy_;

public:
    LegacyPaymentAdapter() : legacy_(std::make_unique<LegacyPaymentProcessor>()) {}

    bool authorize(const std::string &userId, double amount) override
    {
        std::cout << "[Adapter] Translating authorize() to legacy interface\n";
        legacy_->verifyAccountOldWay(userId);
        return true; // Simplified
    }

    bool charge(const std::string &userId, double amount) override
    {
        std::cout << "[Adapter] Translating charge() to legacy interface\n";
        legacy_->processPaymentOldWay(userId, amount);
        return true; // Simplified
    }
};

// ============================================================================
// SOLUTION 2: Class Adapter (multiple inheritance - less preferred)
// ============================================================================
//
// Alternative using inheritance instead of composition. NOT RECOMMENDED because:
// ✗ Tighter coupling (is-a relationship instead of has-a)
// ✗ Can't adapt same class multiple ways
// ✗ Multiple inheritance complexity and virtual method issues
// ✗ Less flexible - hard to swap implementations at runtime
// ✗ Violates composition over inheritance principle

class LegacyPaymentClassAdapter : public IPaymentGateway,
                                  private LegacyPaymentProcessor
{
public:
    bool authorize(const std::string &userId, double amount) override
    {
        std::cout << "[ClassAdapter] Direct inheritance translation\n";
        verifyAccountOldWay(userId);
        return true;
    }

    bool charge(const std::string &userId, double amount) override
    {
        std::cout << "[ClassAdapter] Direct inheritance translation\n";
        processPaymentOldWay(userId, amount);
        return true;
    }
};

// ============================================================================
// Client code - works with IPaymentGateway interface
// ============================================================================

class CheckoutService
{
private:
    IPaymentGateway &gateway_;

public:
    CheckoutService(IPaymentGateway &gateway) : gateway_(gateway) {}

    void processOrder(const std::string &userId, double amount)
    {
        std::cout << "\n=== Processing Order ===\n";
        if (gateway_.authorize(userId, amount))
        {
            std::cout << "Authorization successful\n";
            if (gateway_.charge(userId, amount))
            {
                std::cout << "Charge successful\n";
            }
        }
    }
};

// ============================================================================
// EXAMPLE 2: Logger Library Integration
// ============================================================================

// PROBLEM: Semantic mismatch - What's wrong?
// ---
// Your application was built with a logger that uses semantic method names:
//   - logInfo(message)
//   - logError(message)
//
// Now you're integrating a third-party library with a generic interface:
//   - writeLog(level, message)  // Level: 1=debug, 2=info, 3=error, etc.
//
// Problems without adapter:
// ✗ Hard to remember: "Is 1 info or debug? What's 3?"
// ✗ Client code scattered with magic numbers: writeLog(3, msg) // What does 3 mean?
// ✗ If you switch libraries, all call sites must change
// ✗ No compile-time safety - wrong level number gives silent bugs
// ✗ Code becomes unreadable and error-prone
//
// Example of messy code WITHOUT adapter:
//   thirdParty.writeLog(1, info_message);     // Info?
//   thirdParty.writeLog(3, error_message);    // Error? But what's 3?
//   thirdParty.writeLog(2, warning_message);  // Warning? Or info? Confusion!

// Target interface your application uses
class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void logInfo(const std::string &message) = 0;
    virtual void logError(const std::string &message) = 0;
};

// Third-party logging library with different interface
class ThirdPartyLogger
{
public:
    void writeLog(int level, const std::string &msg)
    {
        std::cout << "[ThirdParty] Level " << level << ": " << msg << "\n";
    }
};

// SOLUTION: Adapter provides semantic interface
// ---
// The adapter:
// 1. Takes semantic method calls (logInfo, logError)
// 2. Translates to third-party levels internally
// 3. Client code is clean, readable, and intent-clear
// 4. All magic number translation in ONE place
//
// Now your code reads naturally:
//   logger->logInfo("User logged in");      // Clear intent!
//   logger->logError("Connection failed");  // Obvious meaning!
//
// Benefits:
// ✓ Code is self-documenting (no magic numbers)
// ✓ Easy to maintain - change mapping in adapter if library changes
// ✓ Reusable across entire codebase
// ✓ Type-safe - compiler catches wrong method names
// ✓ Centralized translation logic

// Adapter to make ThirdPartyLogger work as ILogger
class LoggerAdapter : public ILogger
{
private:
    ThirdPartyLogger thirdParty_;

public:
    void logInfo(const std::string &message) override
    {
        thirdParty_.writeLog(1, message); // Map info to level 1
    }

    void logError(const std::string &message) override
    {
        thirdParty_.writeLog(3, message); // Map error to level 3
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== ADAPTER PATTERN DEMO ===\n";

    // Using Object Adapter
    std::cout << "\n--- Object Adapter (Composition) ---\n";
    {
        LegacyPaymentAdapter adapter;
        CheckoutService checkout(adapter);
        checkout.processOrder("user123", 99.99);
    }

    // Using Class Adapter
    std::cout << "\n--- Class Adapter (Inheritance) ---\n";
    {
        LegacyPaymentClassAdapter adapter;
        CheckoutService checkout(adapter);
        checkout.processOrder("user456", 149.99);
    }

    // Logger adapter example
    std::cout << "\n--- Logger Adapter ---\n";
    {
        LoggerAdapter logger;
        logger.logInfo("Application started");
        logger.logError("Connection failed");
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Object Adapter uses composition (preferred for flexibility)\n";
    std::cout << "2. Class Adapter uses inheritance (tighter coupling)\n";
    std::cout << "3. Clients work with target interface, unaware of adaptation\n";
    std::cout << "4. Enables OCP - extend without modifying existing code\n";
    std::cout << "5. Common in legacy system integration\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Integrate legacy code with new systems
 * - Wrap third-party libraries with incompatible interfaces
 * - Reuse existing classes that don't match required interface
 * - Create reusable code that works with unrelated classes
 *
 * BENEFITS:
 * - Single Responsibility: Separation of interface/protocol conversion from business logic
 * - Open/Closed Principle: Add new adapters without changing client or adaptee
 * - Promotes code reuse
 *
 * DRAWBACKS:
 * - Increased complexity from additional indirection
 * - Sometimes simpler to just modify the source (if you own it)
 *
 * COMPARISON:
 * - Adapter: Makes existing interfaces work together (retrofit)
 * - Bridge: Separates abstraction from implementation (designed upfront)
 * - Facade: Simplifies complex subsystem (not about compatibility)
 * - Decorator: Adds behavior, same interface (not about adaptation)
 */
