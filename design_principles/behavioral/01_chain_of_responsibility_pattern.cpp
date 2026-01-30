/**
 * CHAIN OF RESPONSIBILITY PATTERN
 *
 * Intent: Avoid coupling sender of request to receivers by letting multiple objects
 * handle the request along a chain. Pass the request along the chain until handled.
 *
 * Problem: Multiple handlers, don't know in advance which will handle the request
 * Solution: Chain handlers together, each decides to handle or pass to next
 *
 * Real-world analogy: Customer complaint escalation - customer → agent → manager → director
 *
 * SOLID relation:
 * - SRP: Each handler responsible for one type of request
 * - OCP: Add new handlers without modifying existing ones
 * - DIP: Handlers depend on abstract base, not concrete implementations
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Technical Support Ticket Routing
// ============================================================================

// PROBLEM: Hard-coded request routing logic
// ---
// Support system receives tickets of varying complexity:
// - Level 1: Login issues, password resets
// - Level 2: Feature bugs, performance issues
// - Level 3: System outages, critical data loss
//
// WITHOUT Chain of Responsibility:
// ✗ Main dispatcher has giant switch statement:
//   if (ticket is LOGIN) -> Tier1Handler
//   if (ticket is BUG) -> Tier2Handler
//   if (ticket is OUTAGE) -> Tier3Handler
//
// ✗ Adding new ticket type requires modifying dispatcher
// ✗ Handler selection logic spread throughout codebase
// ✗ Tight coupling - dispatcher knows all handlers
// ✗ Hard to test - must mock entire dispatcher
// ✗ Changing routing logic requires touching main code
//
// Example of messy code WITHOUT pattern:
//   if (priority == LOW) {
//       tier1->handle(ticket);
//   } else if (priority == MEDIUM) {
//       tier2->handle(ticket);
//   } else if (priority == HIGH) {
//       tier3->handle(ticket);
//   } else {
//       director->handle(ticket);
//   }

enum class TicketPriority
{
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

class SupportTicket
{
private:
    std::string id_;
    std::string description_;
    TicketPriority priority_;

public:
    SupportTicket(const std::string &id, const std::string &desc, TicketPriority p)
        : id_(id), description_(desc), priority_(p) {}

    std::string getId() const { return id_; }
    std::string getDescription() const { return description_; }
    TicketPriority getPriority() const { return priority_; }
};

// SOLUTION: Chain of Responsibility - Handlers delegate
// ---
// Key insight: Create chain of handlers where each:
// 1. Checks if it can handle the request
// 2. If YES: processes and STOPS propagation
// 3. If NO: passes to next handler in chain
//
// Benefits:
// ✓ Decoupled - sender doesn't know handlers
// ✓ New handler? Just add to chain
// ✓ Easy to test - test each handler independently
// ✓ Flexible - rearrange chain order at runtime
// ✓ OCP - add handlers without modifying existing code
// ✓ SRP - each handler handles ONE type of request

// Handler interface
class SupportHandler
{
protected:
    std::unique_ptr<SupportHandler> nextHandler_;

public:
    virtual ~SupportHandler() = default;

    void setNext(std::unique_ptr<SupportHandler> next)
    {
        nextHandler_ = std::move(next);
    }

    virtual void handle(const SupportTicket &ticket) = 0;
};

// Level 1: Tier 1 Support (basic issues)
class Tier1Support : public SupportHandler
{
public:
    void handle(const SupportTicket &ticket) override
    {
        if (ticket.getPriority() == TicketPriority::LOW)
        {
            std::cout << "[Tier1] Handling LOW priority ticket " << ticket.getId()
                      << ": " << ticket.getDescription() << "\n";
            return; // Handled, stop chain
        }
        // Can't handle, pass to next
        if (nextHandler_)
        {
            nextHandler_->handle(ticket);
        }
    }
};

// Level 2: Tier 2 Support (medium issues)
class Tier2Support : public SupportHandler
{
public:
    void handle(const SupportTicket &ticket) override
    {
        if (ticket.getPriority() == TicketPriority::MEDIUM)
        {
            std::cout << "[Tier2] Handling MEDIUM priority ticket " << ticket.getId()
                      << ": " << ticket.getDescription() << "\n";
            return; // Handled, stop chain
        }
        // Can't handle, pass to next
        if (nextHandler_)
        {
            nextHandler_->handle(ticket);
        }
    }
};

// Level 3: Tier 3 Support (critical issues)
class Tier3Support : public SupportHandler
{
public:
    void handle(const SupportTicket &ticket) override
    {
        if (ticket.getPriority() == TicketPriority::HIGH)
        {
            std::cout << "[Tier3] Handling HIGH priority ticket " << ticket.getId()
                      << ": " << ticket.getDescription() << "\n";
            return; // Handled, stop chain
        }
        // Can't handle, pass to next
        if (nextHandler_)
        {
            nextHandler_->handle(ticket);
        }
    }
};

// Director: Last resort for critical issues
class Director : public SupportHandler
{
public:
    void handle(const SupportTicket &ticket) override
    {
        std::cout << "[Director] Handling CRITICAL ticket " << ticket.getId()
                  << ": " << ticket.getDescription() << " (escalation complete)\n";
    }
};

// ============================================================================
// EXAMPLE 2: Logger with Different Levels
// ============================================================================

// PROBLEM: Need to filter log messages by level
// ---
// Application has different log levels:
// - DEBUG: detailed info for developers
// - INFO: general information
// - WARNING: potential issues
// - ERROR: errors that need attention
//
// Without Chain:
// ✗ Main logger has if-else for each level
// ✗ Adding new level requires modifying logger
// ✗ Hard to configure which levels go where
// ✗ Filtering logic mixed with output logic

// SOLUTION: Chain handles filtering and output

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
protected:
    std::unique_ptr<Logger> nextLogger_;
    LogLevel level_;

public:
    Logger(LogLevel l) : level_(l) {}
    virtual ~Logger() = default;

    void setNext(std::unique_ptr<Logger> next)
    {
        nextLogger_ = std::move(next);
    }

    void logMessage(const std::string &message, LogLevel level)
    {
        if (level >= level_)
        {
            writeMessage(message, level);
        }
        // Always pass to next logger (unlike support chain)
        if (nextLogger_)
        {
            nextLogger_->logMessage(message, level);
        }
    }

    virtual void writeMessage(const std::string &message, LogLevel level) = 0;
};

class ConsoleLogger : public Logger
{
public:
    ConsoleLogger(LogLevel l) : Logger(l) {}

    void writeMessage(const std::string &message, LogLevel level) override
    {
        std::string levelStr = (level == LogLevel::DEBUG) ? "DEBUG" : (level == LogLevel::INFO)  ? "INFO"
                                                                  : (level == LogLevel::WARNING) ? "WARNING"
                                                                                                 : "ERROR";
        std::cout << "[Console] " << levelStr << ": " << message << "\n";
    }
};

class FileLogger : public Logger
{
public:
    FileLogger(LogLevel l) : Logger(l) {}

    void writeMessage(const std::string &message, LogLevel level) override
    {
        std::string levelStr = (level == LogLevel::DEBUG) ? "DEBUG" : (level == LogLevel::INFO)  ? "INFO"
                                                                  : (level == LogLevel::WARNING) ? "WARNING"
                                                                                                 : "ERROR";
        std::cout << "[File] Writing to log.txt - " << levelStr << ": " << message << "\n";
    }
};

class EmailLogger : public Logger
{
public:
    EmailLogger(LogLevel l) : Logger(l) {}

    void writeMessage(const std::string &message, LogLevel level) override
    {
        std::cout << "[Email] Sending alert email: " << message << "\n";
    }
};

// ============================================================================
// EXAMPLE 3: Request Approval Workflow
// ============================================================================

// PROBLEM: Approval chains with different authority levels
// ---
// Department expense approvals:
// - Supervisor: can approve up to $1,000
// - Manager: can approve up to $10,000
// - Director: can approve up to $50,000
// - CFO: can approve unlimited (if valid)
//
// Without Chain:
// ✗ Caller must determine authority level
// ✗ Loop through authorities manually
// ✗ Hard to change approval limits
// ✗ Approval logic scattered

// SOLUTION: Chain finds right approver

class ExpenseRequest
{
private:
    std::string id_;
    double amount_;
    std::string description_;

public:
    ExpenseRequest(const std::string &id, double amt, const std::string &desc)
        : id_(id), amount_(amt), description_(desc) {}

    std::string getId() const { return id_; }
    double getAmount() const { return amount_; }
    std::string getDescription() const { return description_; }
};

class Approver
{
protected:
    std::unique_ptr<Approver> nextApprover_;
    double approvalLimit_;
    std::string title_;

public:
    Approver(const std::string &t, double limit) : title_(t), approvalLimit_(limit) {}
    virtual ~Approver() = default;

    void setNext(std::unique_ptr<Approver> next)
    {
        nextApprover_ = std::move(next);
    }

    void approveRequest(const ExpenseRequest &request)
    {
        if (request.getAmount() <= approvalLimit_)
        {
            std::cout << "[" << title_ << "] APPROVED expense " << request.getId()
                      << " ($" << request.getAmount() << "): " << request.getDescription() << "\n";
        }
        else if (nextApprover_)
        {
            std::cout << "[" << title_ << "] Forwarding to next level (amount $"
                      << request.getAmount() << " exceeds limit $" << approvalLimit_ << ")\n";
            nextApprover_->approveRequest(request);
        }
        else
        {
            std::cout << "[" << title_ << "] REJECTED: Amount exceeds all approval limits\n";
        }
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== CHAIN OF RESPONSIBILITY PATTERN DEMO ===\n";

    // Support ticket routing
    std::cout << "\n--- Support Ticket Routing ---\n";
    {
        auto chain = std::make_unique<Tier1Support>();
        auto tier2 = std::make_unique<Tier2Support>();
        auto tier3 = std::make_unique<Tier3Support>();
        auto director = std::make_unique<Director>();

        tier3->setNext(std::move(director));
        tier2->setNext(std::move(tier3));
        chain->setNext(std::move(tier2));

        SupportTicket t1("T001", "Can't log in", TicketPriority::LOW);
        SupportTicket t2("T002", "Feature not working", TicketPriority::MEDIUM);
        SupportTicket t3("T003", "System down", TicketPriority::HIGH);
        SupportTicket t4("T004", "Data loss", TicketPriority::CRITICAL);

        chain->handle(t1);
        chain->handle(t2);
        chain->handle(t3);
        chain->handle(t4);
    }

    // Logger chain
    std::cout << "\n--- Logger Chain ---\n";
    {
        auto console = std::make_unique<ConsoleLogger>(LogLevel::DEBUG);
        auto file = std::make_unique<FileLogger>(LogLevel::WARNING);
        auto email = std::make_unique<EmailLogger>(LogLevel::ERROR);

        file->setNext(std::move(email));
        console->setNext(std::move(file));

        console->logMessage("Application started", LogLevel::INFO);
        console->logMessage("Cache miss detected", LogLevel::WARNING);
        console->logMessage("Database connection failed", LogLevel::ERROR);
    }

    // Expense approval workflow
    std::cout << "\n--- Expense Approval Workflow ---\n";
    {
        auto supervisor = std::make_unique<Approver>("Supervisor", 1000);
        auto manager = std::make_unique<Approver>("Manager", 10000);
        auto director = std::make_unique<Approver>("Director", 50000);
        auto cfo = std::make_unique<Approver>("CFO", 1000000);

        director->setNext(std::move(cfo));
        manager->setNext(std::move(director));
        supervisor->setNext(std::move(manager));

        ExpenseRequest r1("EXP001", 500, "Office supplies");
        ExpenseRequest r2("EXP002", 5000, "Equipment purchase");
        ExpenseRequest r3("EXP003", 30000, "Building lease");
        ExpenseRequest r4("EXP004", 200000, "Office expansion");

        supervisor->approveRequest(r1);
        std::cout << "\n";
        supervisor->approveRequest(r2);
        std::cout << "\n";
        supervisor->approveRequest(r3);
        std::cout << "\n";
        supervisor->approveRequest(r4);
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Chain allows request to pass through handlers\n";
    std::cout << "2. Handler decides: handle or pass to next\n";
    std::cout << "3. Decoupled sender from receivers\n";
    std::cout << "4. Easy to add/remove handlers from chain\n";
    std::cout << "5. Runtime chain construction possible\n";
    std::cout << "6. Risk: request may reach end unhandled\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Don't know in advance which object handles request
 * - Multiple handlers, any one may process
 * - Want to avoid coupling between sender and handlers
 * - Need to send request up a hierarchy
 *
 * BENEFITS:
 * - Decouples sender from receivers
 * - SRP - each handler has single responsibility
 * - OCP - add new handlers without modifying existing
 * - Flexible runtime chain construction
 * - Handlers can be reordered/reconfigured
 *
 * DRAWBACKS:
 * - Request may not be handled (reach end of chain)
 * - Hard to debug - no guaranteed handler
 * - Performance - may traverse long chain
 * - Harder to understand flow than direct calls
 *
 * COMPARISON:
 * - Command: Encapsulates request with receiver
 * - ChainOfResponsibility: Pass request through chain of handlers
 * - Iterator: Access elements sequentially without exposing structure
 * - Mediator: Centralizes communication between objects
 */
