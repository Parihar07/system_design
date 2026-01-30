/**
 * STATE PATTERN
 *
 * Intent: Allow an object to alter its behavior when its internal state changes.
 * The object will appear to change its class.
 *
 * Problem: Object behavior changes with its state; code has complex if-else chains
 * Solution: Represent each state as separate class; delegate to current state
 *
 * Real-world analogy: Traffic light changes behavior based on current state
 *
 * SOLID relation:
 * - SRP: Each state handles its own behavior
 * - OCP: Add new states without modifying existing context
 * - DIP: Context depends on State abstraction
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Document Editor State Machine
// ============================================================================

// PROBLEM: Document behavior changes with workflow state
// ---
// Document states:
// - Draft: Can edit, can publish, can delete
// - Published: Can't edit, can unpublish, can't delete
// - Archived: Can't edit, can't publish, can restore only
//
// Without State pattern:
// ✗ Document class has massive conditional logic:
//   if (state == DRAFT) { allowEdit(); }
//   if (state == PUBLISHED) { denyEdit(); }
//   if (state == ARCHIVED) { denyAll(); }
//
// ✗ Every new state requires modifying Document class
// ✗ Violations of SRP and OCP
// ✗ Complex nested if-else statements
// ✗ Hard to understand state transitions
// ✗ Bug-prone when adding new states
//
// Example messy code WITHOUT State pattern:
//   class Document {
//       void edit(string text) {
//           if (state == DRAFT) {
//               content = text;
//           } else if (state == PUBLISHED) {
//               throw Exception("Cannot edit published");
//           } else if (state == ARCHIVED) {
//               throw Exception("Cannot edit archived");
//           }
//       }
//       void publish() {
//           if (state == DRAFT) {
//               state = PUBLISHED;
//           } else if (state == PUBLISHED) {
//               throw Exception("Already published");
//           }
//           // ... more conditions
//       }
//       // Every operation has similar if-else!
//   };

// SOLUTION: State Pattern - Each state is separate class
// ---
// Key insight: Delegate behavior to current state object
// 1. Define State interface with operations
// 2. Create concrete classes for each state
// 3. Context delegates to state
// 4. State can transition to other states
//
// Benefits:
// ✓ Separates state-specific behavior
// ✓ Each state is independent class
// ✓ Easy to add new states (OCP)
// ✓ Eliminates large if-else chains
// ✓ SRP - each state handles one state's behavior
// ✓ Clear state transition logic

// Forward declaration
class Document;

// State interface
class DocumentState
{
public:
    virtual ~DocumentState() = default;
    virtual void edit(Document &doc, const std::string &text) = 0;
    virtual void publish(Document &doc) = 0;
    virtual void archive(Document &doc) = 0;
    virtual void restore(Document &doc) = 0;
    virtual std::string getStateName() const = 0;
};

// Document (context)
class Document
{
private:
    std::unique_ptr<DocumentState> state_;
    std::string content_;
    std::string title_;

public:
    Document(const std::string &title);

    void setState(std::unique_ptr<DocumentState> state)
    {
        state_ = std::move(state);
    }

    void edit(const std::string &text)
    {
        state_->edit(*this, text);
    }

    void publish()
    {
        state_->publish(*this);
    }

    void archive()
    {
        state_->archive(*this);
    }

    void restore()
    {
        state_->restore(*this);
    }

    void setContent(const std::string &text) { content_ = text; }
    std::string getContent() const { return content_; }
    std::string getStateName() const { return state_->getStateName(); }
};

// Draft state - can edit, can publish
class DraftState : public DocumentState
{
public:
    void edit(Document &doc, const std::string &text) override
    {
        std::cout << "[Draft] Editing document: " << text << "\n";
        doc.setContent(text);
    }

    void publish(Document &doc) override
    {
        std::cout << "[Draft] Publishing document\n";
        doc.setState(std::make_unique<PublishedState>());
    }

    void archive(Document &doc) override
    {
        std::cout << "[Draft] Archiving draft document\n";
        doc.setState(std::make_unique<ArchivedState>());
    }

    void restore(Document &doc) override
    {
        std::cout << "[Draft] Cannot restore from draft state\n";
    }

    std::string getStateName() const override { return "DRAFT"; }
};

// Published state - can't edit, can unpublish/archive
class PublishedState : public DocumentState
{
public:
    void edit(Document &doc, const std::string &text) override
    {
        std::cout << "[Published] ERROR: Cannot edit published document!\n";
    }

    void publish(Document &doc) override
    {
        std::cout << "[Published] ERROR: Already published!\n";
    }

    void archive(Document &doc) override
    {
        std::cout << "[Published] Archiving published document\n";
        doc.setState(std::make_unique<ArchivedState>());
    }

    void restore(Document &doc) override
    {
        std::cout << "[Published] Returning to draft\n";
        doc.setState(std::make_unique<DraftState>());
    }

    std::string getStateName() const override { return "PUBLISHED"; }
};

// Archived state - can't edit/publish, can restore only
class ArchivedState : public DocumentState
{
public:
    void edit(Document &doc, const std::string &text) override
    {
        std::cout << "[Archived] ERROR: Cannot edit archived document!\n";
    }

    void publish(Document &doc) override
    {
        std::cout << "[Archived] ERROR: Cannot publish archived document!\n";
    }

    void archive(Document &doc) override
    {
        std::cout << "[Archived] ERROR: Already archived!\n";
    }

    void restore(Document &doc) override
    {
        std::cout << "[Archived] Restoring to draft\n";
        doc.setState(std::make_unique<DraftState>());
    }

    std::string getStateName() const override { return "ARCHIVED"; }
};

// Constructor implementation
Document::Document(const std::string &title) : title_(title)
{
    setState(std::make_unique<DraftState>());
}

// ============================================================================
// EXAMPLE 2: TCP Connection States
// ============================================================================

// PROBLEM: TCP connection has complex state machine
// ---
// States: Closed -> Listen -> Established -> Close_Wait -> Closed
// Each state allows different operations
//
// SOLUTION: Each state handles its transitions

class TCPConnection;

class TCPConnectionState
{
public:
    virtual ~TCPConnectionState() = default;
    virtual void open(TCPConnection &conn) = 0;
    virtual void send(TCPConnection &conn, const std::string &data) = 0;
    virtual void close(TCPConnection &conn) = 0;
    virtual std::string getStateName() const = 0;
};

class TCPConnection
{
private:
    std::unique_ptr<TCPConnectionState> state_;

public:
    TCPConnection() : state_(std::make_unique<ClosedState>()) {}

    void setState(std::unique_ptr<TCPConnectionState> state)
    {
        std::cout << "    [TCP] Transitioning from " << state_->getStateName() << " to ";
        state_ = std::move(state);
        std::cout << state_->getStateName() << "\n";
    }

    void open() { state_->open(*this); }
    void send(const std::string &data) { state_->send(*this, data); }
    void close() { state_->close(*this); }
    std::string getStateName() const { return state_->getStateName(); }
};

class ClosedState : public TCPConnectionState
{
public:
    void open(TCPConnection &conn) override
    {
        std::cout << "  [Closed->Listen] Opening connection\n";
        conn.setState(std::make_unique<ListenState>());
    }
    void send(TCPConnection &conn, const std::string &data) override
    {
        std::cout << "  [Closed] ERROR: Cannot send on closed connection\n";
    }
    void close(TCPConnection &conn) override
    {
        std::cout << "  [Closed] Already closed\n";
    }
    std::string getStateName() const override { return "Closed"; }
};

class ListenState : public TCPConnectionState
{
public:
    void open(TCPConnection &conn) override
    {
        std::cout << "  [Listen] ERROR: Already listening\n";
    }
    void send(TCPConnection &conn, const std::string &data) override
    {
        std::cout << "  [Listen->Established] Connection established, sending: " << data << "\n";
        conn.setState(std::make_unique<EstablishedState>());
    }
    void close(TCPConnection &conn) override
    {
        std::cout << "  [Listen->Closed] Closing\n";
        conn.setState(std::make_unique<ClosedState>());
    }
    std::string getStateName() const override { return "Listen"; }
};

class EstablishedState : public TCPConnectionState
{
public:
    void open(TCPConnection &conn) override
    {
        std::cout << "  [Established] ERROR: Already established\n";
    }
    void send(TCPConnection &conn, const std::string &data) override
    {
        std::cout << "  [Established] Sending: " << data << "\n";
    }
    void close(TCPConnection &conn) override
    {
        std::cout << "  [Established->CloseWait] Initiating close\n";
        conn.setState(std::make_unique<CloseWaitState>());
    }
    std::string getStateName() const override { return "Established"; }
};

class CloseWaitState : public TCPConnectionState
{
public:
    void open(TCPConnection &conn) override
    {
        std::cout << "  [CloseWait] ERROR: Cannot open during close\n";
    }
    void send(TCPConnection &conn, const std::string &data) override
    {
        std::cout << "  [CloseWait] ERROR: Cannot send during close\n";
    }
    void close(TCPConnection &conn) override
    {
        std::cout << "  [CloseWait->Closed] Connection closed\n";
        conn.setState(std::make_unique<ClosedState>());
    }
    std::string getStateName() const override { return "CloseWait"; }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== STATE PATTERN DEMO ===\n";

    // Document workflow
    std::cout << "\n--- Document Workflow ---\n";
    {
        Document doc("Design Patterns");
        std::cout << "Initial state: " << doc.getStateName() << "\n";

        doc.edit("Chapter 1: Creational patterns...");
        doc.edit("Chapter 2: Structural patterns..."); // Still in draft

        doc.publish();
        std::cout << "Current state: " << doc.getStateName() << "\n";

        doc.edit("Trying to edit"); // Should fail

        doc.archive();
        std::cout << "Current state: " << doc.getStateName() << "\n";

        doc.restore();
        std::cout << "Current state: " << doc.getStateName() << "\n";
    }

    // TCP connection states
    std::cout << "\n--- TCP Connection State Machine ---\n";
    {
        TCPConnection conn;
        std::cout << "Initial state: " << conn.getStateName() << "\n";

        std::cout << "\nOpening connection:\n";
        conn.open();

        std::cout << "\nSending data:\n";
        conn.send("GET / HTTP/1.1");

        std::cout << "\nSending more data:\n";
        conn.send("Host: example.com");

        std::cout << "\nClosing connection:\n";
        conn.close();

        std::cout << "\nTrying to send on closed connection:\n";
        conn.send("This will fail");

        std::cout << "\nFinal state: " << conn.getStateName() << "\n";
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Each state is separate class\n";
    std::cout << "2. Context delegates to current state\n";
    std::cout << "3. State can transition to other states\n";
    std::cout << "4. Eliminates large if-else chains\n";
    std::cout << "5. Easy to add new states\n";
    std::cout << "6. Implements state machine pattern\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Object behavior varies based on internal state
 * - Complex if-else chains on state
 * - State-specific operations scattered throughout
 * - Multiple states with different valid operations
 * - State transitions have complex logic
 *
 * BENEFITS:
 * - Separates state-specific behavior
 * - Eliminates complex conditional logic
 * - Each state in separate class (SRP)
 * - Easy to add new states (OCP)
 * - Clear state transition logic
 * - Easier to understand and maintain
 *
 * DRAWBACKS:
 * - May require many state classes
 * - Increased memory usage
 * - More indirection/method calls
 * - Overkill for simple state machines
 *
 * COMPARISON:
 * - State: Changes behavior based on internal state
 * - Strategy: Client chooses algorithm
 * - Visitor: Perform operations on object structure
 * - Template Method: Skeleton of algorithm varies
 *
 * RELATED:
 * - State Machine: Formal representation of states
 * - Finite State Machine (FSM): Mathematical model
 * - Activity Diagram: UML representation
 */
