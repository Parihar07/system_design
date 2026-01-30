/**
 * MEMENTO PATTERN
 *
 * Intent: Capture and externalize an object's internal state without violating
 * encapsulation, and restore the object to this state later.
 *
 * Problem: Need to save/restore object state without exposing internal structure
 * Solution: Create memento that captures state; originator can restore from it
 *
 * Real-world analogy: Snapshot of game state - can restore to previous checkpoint
 *
 * SOLID relation:
 * - SRP: Originator creates state, Memento stores state, Caretaker manages mementos
 * - Encapsulation: Memento keeps internal state private
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>

// ============================================================================
// EXAMPLE 1: Text Editor Undo/Redo
// ============================================================================

// PROBLEM: Need to save/restore editor state without exposing implementation
// ---
// Text editor needs:
// - Save current state
// - Restore to previous state
// - Support multiple undo/redo
//
// Without Memento:
// ✗ Editor must expose internal state
// ✗ Client knows implementation details
// ✗ State saving scattered through code
// ✗ No encapsulation
// ✗ Hard to maintain
//
// SOLUTION: Memento Pattern - Capture and restore
// ---
// Three actors:
// 1. Originator: Creates memento, restores from memento
// 2. Memento: Stores state snapshot
// 3. Caretaker: Manages memento history

// Memento - stores captured state
class TextEditorMemento
{
private:
    std::string content_;
    int cursorPos_;
    std::string timestamp_;

public:
    TextEditorMemento(const std::string &content, int cursor)
        : content_(content), cursorPos_(cursor)
    {
        // Generate timestamp
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        timestamp_ = oss.str();
    }

    std::string getContent() const { return content_; }
    int getCursorPos() const { return cursorPos_; }
    std::string getTimestamp() const { return timestamp_; }
};

// Originator - creates and restores from memento
class TextEditor
{
private:
    std::string content_;
    int cursorPos_;

public:
    TextEditor() : cursorPos_(0) {}

    void insertText(const std::string &text)
    {
        content_.insert(cursorPos_, text);
        cursorPos_ += text.length();
    }

    void deleteChar()
    {
        if (cursorPos_ > 0 && cursorPos_ <= static_cast<int>(content_.length()))
        {
            content_.erase(cursorPos_ - 1, 1);
            cursorPos_--;
        }
    }

    void setCursorPos(int pos)
    {
        cursorPos_ = std::max(0, std::min(pos, static_cast<int>(content_.length())));
    }

    // Save state to memento
    std::unique_ptr<TextEditorMemento> saveState() const
    {
        return std::make_unique<TextEditorMemento>(content_, cursorPos_);
    }

    // Restore state from memento
    void restoreState(const TextEditorMemento &memento)
    {
        content_ = memento.getContent();
        cursorPos_ = memento.getCursorPos();
    }

    std::string getContent() const { return content_; }
    int getCursorPos() const { return cursorPos_; }

    void display() const
    {
        std::cout << "    Content: \"" << content_ << "\"\n";
        std::cout << "    Cursor pos: " << cursorPos_ << "\n";
    }
};

// Caretaker - manages memento history
class TextEditorHistory
{
private:
    std::vector<std::unique_ptr<TextEditorMemento>> history_;
    int currentIndex_ = -1;

public:
    void saveState(const TextEditor &editor)
    {
        // Remove any redo history when new save is made
        if (currentIndex_ < static_cast<int>(history_.size()) - 1)
        {
            history_.erase(history_.begin() + currentIndex_ + 1, history_.end());
        }

        history_.push_back(editor.saveState());
        currentIndex_++;
        std::cout << "    [History] State saved (total: " << history_.size() << ")\n";
    }

    bool undo()
    {
        if (currentIndex_ > 0)
        {
            currentIndex_--;
            std::cout << "    [History] Undo (position: " << currentIndex_ << ")\n";
            return true;
        }
        return false;
    }

    bool redo()
    {
        if (currentIndex_ < static_cast<int>(history_.size()) - 1)
        {
            currentIndex_++;
            std::cout << "    [History] Redo (position: " << currentIndex_ << ")\n";
            return true;
        }
        return false;
    }

    const TextEditorMemento &getCurrentState() const
    {
        return *history_[currentIndex_];
    }

    bool canUndo() const { return currentIndex_ > 0; }
    bool canRedo() const { return currentIndex_ < static_cast<int>(history_.size()) - 1; }
};

// ============================================================================
// EXAMPLE 2: Game Checkpoint System
// ============================================================================

// PROBLEM: Save game state for checkpoints/restart
// ---
// Game state: position, health, inventory, score
//
// SOLUTION: Create memento for each checkpoint

class GameStateMemento
{
private:
    int level_;
    int health_;
    int score_;
    std::string position_;

public:
    GameStateMemento(int level, int health, int score, const std::string &pos)
        : level_(level), health_(health), score_(score), position_(pos) {}

    int getLevel() const { return level_; }
    int getHealth() const { return health_; }
    int getScore() const { return score_; }
    std::string getPosition() const { return position_; }
};

class Game
{
private:
    int level_;
    int health_;
    int score_;
    std::string position_;

public:
    Game() : level_(1), health_(100), score_(0), position_("Start") {}

    void move(const std::string &newPos)
    {
        position_ = newPos;
        health_ -= 10; // Moving costs health
        score_ += 10;
    }

    std::unique_ptr<GameStateMemento> createCheckpoint()
    {
        return std::make_unique<GameStateMemento>(level_, health_, score_, position_);
    }

    void restoreCheckpoint(const GameStateMemento &checkpoint)
    {
        level_ = checkpoint.getLevel();
        health_ = checkpoint.getHealth();
        score_ = checkpoint.getScore();
        position_ = checkpoint.getPosition();
    }

    void display() const
    {
        std::cout << "    Level: " << level_ << ", Health: " << health_
                  << ", Score: " << score_ << ", Position: " << position_ << "\n";
    }
};

// ============================================================================
// EXAMPLE 3: Configuration Snapshot
// ============================================================================

// PROBLEM: Need to save/restore configuration
// ---
// Application configuration snapshot

class ConfigMemento
{
private:
    int brightness_;
    int volume_;
    std::string theme_;
    bool notifications_;

public:
    ConfigMemento(int bright, int vol, const std::string &t, bool notif)
        : brightness_(bright), volume_(vol), theme_(t), notifications_(notif) {}

    int getBrightness() const { return brightness_; }
    int getVolume() const { return volume_; }
    std::string getTheme() const { return theme_; }
    bool getNotifications() const { return notifications_; }
};

class AppConfig
{
private:
    int brightness_ = 75;
    int volume_ = 50;
    std::string theme_ = "light";
    bool notifications_ = true;

public:
    void setBrightness(int b) { brightness_ = b; }
    void setVolume(int v) { volume_ = v; }
    void setTheme(const std::string &t) { theme_ = t; }
    void setNotifications(bool n) { notifications_ = n; }

    std::unique_ptr<ConfigMemento> save() const
    {
        return std::make_unique<ConfigMemento>(brightness_, volume_, theme_, notifications_);
    }

    void restore(const ConfigMemento &memento)
    {
        brightness_ = memento.getBrightness();
        volume_ = memento.getVolume();
        theme_ = memento.getTheme();
        notifications_ = memento.getNotifications();
    }

    void display() const
    {
        std::cout << "    Brightness: " << brightness_ << ", Volume: " << volume_
                  << ", Theme: " << theme_ << ", Notifications: "
                  << (notifications_ ? "ON" : "OFF") << "\n";
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== MEMENTO PATTERN DEMO ===\n";

    // Text editor undo/redo
    std::cout << "\n--- Text Editor with Undo/Redo ---\n";
    {
        TextEditor editor;
        TextEditorHistory history;

        std::cout << "Initial state:\n";
        editor.display();
        history.saveState(editor);

        std::cout << "\nInserting text:\n";
        editor.insertText("Hello");
        editor.display();
        history.saveState(editor);

        std::cout << "\nInserting more text:\n";
        editor.insertText(" World");
        editor.display();
        history.saveState(editor);

        std::cout << "\nDeleting character:\n";
        editor.deleteChar();
        editor.display();
        history.saveState(editor);

        std::cout << "\nUndoing last change:\n";
        if (history.canUndo())
        {
            history.undo();
            editor.restoreState(history.getCurrentState());
            editor.display();
        }

        std::cout << "\nUndoing again:\n";
        if (history.canUndo())
        {
            history.undo();
            editor.restoreState(history.getCurrentState());
            editor.display();
        }

        std::cout << "\nRedoing:\n";
        if (history.canRedo())
        {
            history.redo();
            editor.restoreState(history.getCurrentState());
            editor.display();
        }
    }

    // Game checkpoints
    std::cout << "\n--- Game Checkpoint System ---\n";
    {
        Game game;
        std::vector<std::unique_ptr<GameStateMemento>> checkpoints;

        std::cout << "Starting game:\n";
        game.display();
        checkpoints.push_back(game.createCheckpoint());

        std::cout << "\nMoving to Forest:\n";
        game.move("Forest");
        game.display();

        std::cout << "\nMoving to Cave:\n";
        game.move("Cave");
        game.display();

        std::cout << "\nMoving to Dragon:\n";
        game.move("Dragon");
        game.display();

        std::cout << "\nRestoring checkpoint:\n";
        game.restoreCheckpoint(*checkpoints[0]);
        game.display();
    }

    // Configuration snapshots
    std::cout << "\n--- Configuration Snapshots ---\n";
    {
        AppConfig config;

        std::cout << "Default config:\n";
        config.display();
        auto snapshot1 = config.save();

        std::cout << "\nChanging settings:\n";
        config.setBrightness(100);
        config.setVolume(100);
        config.setTheme("dark");
        config.display();
        auto snapshot2 = config.save();

        std::cout << "\nRestoring previous snapshot:\n";
        config.restore(*snapshot1);
        config.display();
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Capture object state without exposing internals\n";
    std::cout << "2. Memento stores snapshot\n";
    std::cout << "3. Originator creates and restores mementos\n";
    std::cout << "4. Caretaker manages memento history\n";
    std::cout << "5. Support undo/redo functionality\n";
    std::cout << "6. Maintains encapsulation\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Need to save and restore object state
 * - Undo/redo functionality
 * - Checkpoints in games
 * - Transaction rollback
 * - Configuration snapshots
 * - Don't want to expose internal state
 *
 * BENEFITS:
 * - Captures state without breaking encapsulation
 * - Preserves encapsulation boundaries
 * - Easy to implement undo/redo
 * - Multiple snapshots for comparison
 * - Caretaker manages state history
 *
 * DRAWBACKS:
 * - Memory overhead (storing multiple states)
 * - Performance cost for large objects
 * - Serialization complexity for complex objects
 * - Memento becomes outdated if originator changes
 *
 * VARIATIONS:
 * - Shallow vs Deep copy
 * - Serialization-based memento
 * - Selective state capture
 * - Memento compression
 *
 * COMPARISON:
 * - Memento: Save/restore state
 * - Prototype: Clone object
 * - Command: Encapsulate request with undo
 *
 * RELATED:
 * - Undo/Redo pattern
 * - Command pattern with undo
 * - Snapshot architecture
 */
