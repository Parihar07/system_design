/**
 * COMMAND PATTERN
 *
 * Intent: Encapsulate a request as an object, allowing parameterization of clients
 * with different requests, queuing, logging, and undoable operations.
 *
 * Problem: Need to decouple objects that invoke operations from those that perform them
 * Solution: Encapsulate request as object with receiver and action
 *
 * Real-world analogy: Restaurant order - customer makes order, chef executes
 *
 * SOLID relation:
 * - SRP: Separate command from execution
 * - OCP: Add new commands without modifying invoker
 * - DIP: Invoker depends on Command abstraction, not concrete operations
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <deque>

// ============================================================================
// EXAMPLE 1: Remote Control Problem
// ============================================================================

// PROBLEM: Tight coupling between remote and devices
// ---
// Simple remote controls specific operations on specific devices:
//   - PressLightButton() -> turn on light
//   - PressFanButton() -> turn on fan
//   - PressACButton() -> turn on AC
//
// Without Command pattern:
// ✗ Remote hardcoded to know about Light, Fan, AC
// ✗ Adding new device requires modifying Remote class
// ✗ Can't undo actions (no history)
// ✗ Can't queue commands for later
// ✗ Can't log/replay command history
// ✗ Tight coupling - Remote changed = test all devices
//
// Example messy code WITHOUT Command:
//   class Remote {
//       Light light_;  // hardcoded
//       Fan fan_;      // hardcoded
//       AC ac_;        // hardcoded
//
//       void pressLightButton() {
//           light_.on();  // tight coupling
//       }
//   };
//
// Problems:
// ✗ Can't swap Light implementation
// ✗ Can't generalize button behavior
// ✗ No undo capability
// ✗ No command history

// SOLUTION: Command Pattern - Encapsulate action
// ---
// Key insight: Each button press = Command object
// 1. Command knows what to do (execute)
// 2. Command knows how to undo it
// 3. Remote just invokes command (doesn't care what it does)
// 4. Can queue, log, replay commands
//
// Benefits:
// ✓ Decouple remote from devices
// ✓ Add new commands without touching remote
// ✓ Support undo/redo
// ✓ Queue commands for later
// ✓ Log command history
// ✓ Replay commands

// Devices (receivers)
class Light
{
public:
    void on() { std::cout << "[Light] Turning ON\n"; }
    void off() { std::cout << "[Light] Turning OFF\n"; }
};

class Fan
{
public:
    void on() { std::cout << "[Fan] Turning ON\n"; }
    void off() { std::cout << "[Fan] Turning OFF\n"; }
    void setSpeed(int speed) { std::cout << "[Fan] Setting speed to " << speed << "\n"; }
};

class AC
{
public:
    void on() { std::cout << "[AC] Turning ON\n"; }
    void off() { std::cout << "[AC] Turning OFF\n"; }
    void setTemperature(int temp) { std::cout << "[AC] Setting temp to " << temp << "°C\n"; }
};

// Command interface
class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Concrete commands
class LightOnCommand : public Command
{
private:
    Light &light_;

public:
    LightOnCommand(Light &l) : light_(l) {}
    void execute() override { light_.on(); }
    void undo() override { light_.off(); }
};

class LightOffCommand : public Command
{
private:
    Light &light_;

public:
    LightOffCommand(Light &l) : light_(l) {}
    void execute() override { light_.off(); }
    void undo() override { light_.on(); }
};

class FanOnCommand : public Command
{
private:
    Fan &fan_;

public:
    FanOnCommand(Fan &f) : fan_(f) {}
    void execute() override { fan_.on(); }
    void undo() override { fan_.off(); }
};

class ACSetTemperatureCommand : public Command
{
private:
    AC &ac_;
    int temperature_;
    int previousTemp_;

public:
    ACSetTemperatureCommand(AC &a, int temp) : ac_(a), temperature_(temp), previousTemp_(22) {}
    void execute() override
    {
        previousTemp_ = 22; // Simplified - normally read current
        ac_.setTemperature(temperature_);
    }
    void undo() override { ac_.setTemperature(previousTemp_); }
};

// No-op command for unassigned buttons
class NoCommand : public Command
{
public:
    void execute() override {}
    void undo() override {}
};

// Remote (invoker)
class RemoteControl
{
private:
    static const int SLOT_COUNT = 7;
    std::unique_ptr<Command> commands_[SLOT_COUNT];
    std::unique_ptr<Command> lastCommand_;
    std::deque<std::unique_ptr<Command>> history_;

public:
    RemoteControl() : lastCommand_(std::make_unique<NoCommand>())
    {
        for (int i = 0; i < SLOT_COUNT; ++i)
        {
            commands_[i] = std::make_unique<NoCommand>();
        }
    }

    void setCommand(int slot, std::unique_ptr<Command> cmd)
    {
        if (slot >= 0 && slot < SLOT_COUNT)
        {
            commands_[slot] = std::move(cmd);
        }
    }

    void pressButton(int slot)
    {
        if (slot >= 0 && slot < SLOT_COUNT)
        {
            commands_[slot]->execute();
            // Don't store NoCommand
            if (dynamic_cast<NoCommand *>(commands_[slot].get()) == nullptr)
            {
                // Store for undo (simplified - would need to clone)
                std::cout << "[Remote] Command stored in history\n";
            }
        }
    }

    void pressUndo()
    {
        lastCommand_->undo();
    }
};

// ============================================================================
// EXAMPLE 2: Undo/Redo System
// ============================================================================

// PROBLEM: No way to undo operations
// ---
// Text editor with operations:
// - Type text
// - Delete text
// - Format text
//
// Without Command:
// ✗ Each operation must store previous state
// ✗ Undo logic scattered throughout code
// ✗ Complex state management
// ✗ Hard to redo after undo
//
// SOLUTION: Command stores both execute and undo

class TextEditor
{
private:
    std::string content_;

public:
    void insertText(const std::string &text)
    {
        content_ += text;
        std::cout << "[Editor] Content: \"" << content_ << "\"\n";
    }
    void deleteText(int count)
    {
        if (count <= static_cast<int>(content_.length()))
        {
            content_ = content_.substr(0, content_.length() - count);
        }
        std::cout << "[Editor] Content: \"" << content_ << "\"\n";
    }
    std::string getContent() const { return content_; }
};

class InsertTextCommand : public Command
{
private:
    TextEditor &editor_;
    std::string text_;

public:
    InsertTextCommand(TextEditor &e, const std::string &t) : editor_(e), text_(t) {}
    void execute() override { editor_.insertText(text_); }
    void undo() override { editor_.deleteText(text_.length()); }
};

// ============================================================================
// EXAMPLE 3: Macro Recording
// ============================================================================

// PROBLEM: Can't record and replay command sequences
// ---
// Need to record multiple commands and replay them:
//   1. Turn on light
//   2. Set AC to 22°C
//   3. Turn on fan
//
// Without Command:
// ✗ No way to record arbitrary action sequences
// ✗ Each device has different interface
// ✗ Coupling to specific devices
//
// SOLUTION: Store commands and execute them

class Macro
{
private:
    std::vector<std::unique_ptr<Command>> commands_;

public:
    void addCommand(std::unique_ptr<Command> cmd)
    {
        commands_.push_back(std::move(cmd));
    }
    void execute()
    {
        std::cout << "[Macro] Executing " << commands_.size() << " commands:\n";
        for (auto &cmd : commands_)
        {
            cmd->execute();
        }
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== COMMAND PATTERN DEMO ===\n";

    // Remote control
    std::cout << "\n--- Remote Control ---\n";
    {
        Light light;
        Fan fan;
        AC ac;
        RemoteControl remote;

        // Program remote
        remote.setCommand(0, std::make_unique<LightOnCommand>(light));
        remote.setCommand(1, std::make_unique<FanOnCommand>(fan));
        remote.setCommand(2, std::make_unique<ACSetTemperatureCommand>(ac, 20));

        // Use remote
        std::cout << "Pressing button 0:\n";
        remote.pressButton(0);
        std::cout << "Pressing button 1:\n";
        remote.pressButton(1);
        std::cout << "Pressing button 2:\n";
        remote.pressButton(2);
    }

    // Undo/Redo
    std::cout << "\n--- Undo/Redo ---\n";
    {
        TextEditor editor;
        std::vector<std::unique_ptr<Command>> history;

        std::cout << "Inserting text:\n";
        auto cmd1 = std::make_unique<InsertTextCommand>(editor, "Hello");
        cmd1->execute();
        history.push_back(std::move(cmd1));

        std::cout << "Inserting more text:\n";
        auto cmd2 = std::make_unique<InsertTextCommand>(editor, " World");
        cmd2->execute();
        history.push_back(std::move(cmd2));

        std::cout << "\nUndoing last command:\n";
        history.back()->undo();

        std::cout << "Undoing previous command:\n";
        history[0]->undo();
    }

    // Macro recording
    std::cout << "\n--- Macro Recording ---\n";
    {
        Light light;
        Fan fan;
        AC ac;

        Macro macro;
        macro.addCommand(std::make_unique<LightOnCommand>(light));
        macro.addCommand(std::make_unique<FanOnCommand>(fan));
        macro.addCommand(std::make_unique<ACSetTemperatureCommand>(ac, 22));

        std::cout << "Recording and replaying macro:\n";
        macro.execute();

        std::cout << "\nReplaying macro again:\n";
        macro.execute();
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Encapsulate request as object\n";
    std::cout << "2. Decouple invoker from receiver\n";
    std::cout << "3. Support undo/redo operations\n";
    std::cout << "4. Queue and log commands\n";
    std::cout << "5. Macro recording capability\n";
    std::cout << "6. Easy to add new commands\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Parameterize objects by action (callbacks)
 * - Queue operations for later execution
 * - Support undo/redo functionality
 * - Log operations and replay them
 * - Structure system around operations
 * - Support transaction/atomic operations
 *
 * BENEFITS:
 * - Decouples invoker from receiver
 * - Encapsulates requests as objects
 * - Can queue and schedule commands
 * - Easy to add new commands (OCP)
 * - Support undo/redo/replay
 * - Can create composite commands
 *
 * DRAWBACKS:
 * - More classes and objects
 * - Increased memory usage
 * - Added complexity for simple cases
 *
 * COMPARISON:
 * - Command: Encapsulates request, defers execution
 * - Strategy: Encapsulates algorithm, immediate execution
 * - Callback: Function pointer/lambda for single action
 * - Chain of Responsibility: Pass request through chain
 */
