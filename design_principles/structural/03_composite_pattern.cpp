/**
 * COMPOSITE PATTERN
 *
 * Intent: Compose objects into tree structures to represent part-whole hierarchies.
 * Lets clients treat individual objects and compositions uniformly.
 *
 * Problem: You need to work with tree-like structures and want to treat leaves and
 * branches uniformly.
 * Solution: Define a common interface for both leaf and composite objects.
 *
 * Real-world analogy: File system - folders contain files/folders, treat both as "items"
 *
 * SOLID relation:
 * - OCP: Add new component types without changing existing code
 * - LSP: Leaf and Composite both substitute for Component
 * - Single interface for part and whole
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// ============================================================================
// EXAMPLE 1: File System Problem
// ============================================================================

// PROBLEM: Treating files and directories differently
// ---
// File system has:
// - Files: have size, can be displayed
// - Directories: contain other files/directories, calculate total size recursively
//
// Without Composite pattern:
// ✗ Client must check: is this a File or Directory?
// ✗ If File: access size directly
// ✗ If Directory: must iterate and sum children recursively
// ✗ Scattered type checking everywhere
//
// Example messy code WITHOUT Composite:
//   if (item is File) {
//       size = item.getSize();
//   } else if (item is Directory) {
//       size = 0;
//       for (child : item.getChildren()) {
//           if (child is File) size += child.getSize();
//           else if (child is Directory) size += getDirectorySize(child); // recursion
//       }
//   }
//
// Problems:
// ✗ Type checking scattered throughout codebase (violates DRY, OCP)
// ✗ Complex recursive logic duplicated everywhere
// ✗ Adding new operations requires modifying all type-check sites
// ✗ Hard to maintain - changing either type breaks everything
// ✗ Violates Single Responsibility Principle
// ✗ Client code becomes unreadable and error-prone
//
// SOLUTION: Composite Pattern - Uniform interface
// ---
// Key insight: Both files and directories are "file system components"
//
// Solution:
// 1. Define common interface: FileSystemComponent
// 2. File implements: display(), getSize()
// 3. Directory implements: display(), getSize() + child management
// 4. Client code treats both uniformly - NO TYPE CHECKING!
//
// Now clean client code:
//   component->display();  // Works for File OR Directory - client doesn't care!
//   int size = component->getSize();  // Same call for both!
//
// Benefits:
// ✓ NO type checking - client code is clean and simple
// ✓ Recursive behavior comes naturally
// ✓ Easy to add new operation - implement once in interface
// ✓ Follows OCP - add new component types without changing client
// ✓ Treats part (file) and whole (directory) identically

// Component interface
class FileSystemComponent
{
protected:
    std::string name_;

public:
    explicit FileSystemComponent(const std::string &name) : name_(name) {}
    virtual ~FileSystemComponent() = default;

    virtual void display(int depth = 0) const = 0;
    virtual int getSize() const = 0;
    virtual std::string getName() const { return name_; }

    // Operations for composite (default implementation throws or no-op)
    virtual void add(std::shared_ptr<FileSystemComponent> component)
    {
        throw std::runtime_error("Cannot add to leaf component");
    }

    virtual void remove(std::shared_ptr<FileSystemComponent> component)
    {
        throw std::runtime_error("Cannot remove from leaf component");
    }

    virtual std::shared_ptr<FileSystemComponent> getChild(int index)
    {
        throw std::runtime_error("No children in leaf component");
    }
};

// Leaf: File
class File : public FileSystemComponent
{
private:
    int size_;

public:
    File(const std::string &name, int size)
        : FileSystemComponent(name), size_(size) {}

    void display(int depth = 0) const override
    {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "📄 " << name_ << " (" << size_ << " KB)\n";
    }

    int getSize() const override
    {
        return size_;
    }
};

// Composite: Directory
class Directory : public FileSystemComponent
{
private:
    std::vector<std::shared_ptr<FileSystemComponent>> children_;

public:
    explicit Directory(const std::string &name) : FileSystemComponent(name) {}

    void add(std::shared_ptr<FileSystemComponent> component) override
    {
        children_.push_back(component);
    }

    void remove(std::shared_ptr<FileSystemComponent> component) override
    {
        children_.erase(
            std::remove(children_.begin(), children_.end(), component),
            children_.end());
    }

    std::shared_ptr<FileSystemComponent> getChild(int index) override
    {
        if (index >= 0 && index < static_cast<int>(children_.size()))
        {
            return children_[index];
        }
        return nullptr;
    }

    void display(int depth = 0) const override
    {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "📁 " << name_ << "/\n";
        for (const auto &child : children_)
        {
            child->display(depth + 1);
        }
    }

    int getSize() const override
    {
        int totalSize = 0;
        for (const auto &child : children_)
        {
            totalSize += child->getSize();
        }
        return totalSize;
    }
};

// ============================================================================
// EXAMPLE 2: UI Component hierarchy (widgets)
// ============================================================================

class UIComponent
{
protected:
    std::string name_;

public:
    explicit UIComponent(const std::string &name) : name_(name) {}
    virtual ~UIComponent() = default;

    virtual void render() const = 0;
    virtual void handleEvent(const std::string &event) = 0;

    virtual void add(std::shared_ptr<UIComponent> component)
    {
        throw std::runtime_error("Cannot add to leaf UI component");
    }
};

// Leaf components
class Button : public UIComponent
{
public:
    explicit Button(const std::string &label) : UIComponent(label) {}

    void render() const override
    {
        std::cout << "  [Button: " << name_ << "]\n";
    }

    void handleEvent(const std::string &event) override
    {
        std::cout << "  Button '" << name_ << "' handling: " << event << "\n";
    }
};

class TextBox : public UIComponent
{
private:
    std::string content_;

public:
    explicit TextBox(const std::string &name) : UIComponent(name) {}

    void render() const override
    {
        std::cout << "  [TextBox: " << name_ << " = \"" << content_ << "\"]\n";
    }

    void handleEvent(const std::string &event) override
    {
        if (event == "input")
        {
            content_ += "x";
            std::cout << "  TextBox '" << name_ << "' input received\n";
        }
    }
};

// Composite component
class Panel : public UIComponent
{
private:
    std::vector<std::shared_ptr<UIComponent>> children_;

public:
    explicit Panel(const std::string &name) : UIComponent(name) {}

    void add(std::shared_ptr<UIComponent> component) override
    {
        children_.push_back(component);
    }

    void render() const override
    {
        std::cout << "┌─ Panel: " << name_ << " ─┐\n";
        for (const auto &child : children_)
        {
            child->render();
        }
        std::cout << "└─────────────────┘\n";
    }

    void handleEvent(const std::string &event) override
    {
        std::cout << "Panel '" << name_ << "' propagating: " << event << "\n";
        for (const auto &child : children_)
        {
            child->handleEvent(event);
        }
    }
};

// ============================================================================
// EXAMPLE 3: Organization hierarchy (employees)
// ============================================================================

class Employee
{
protected:
    std::string name_;
    std::string position_;
    double salary_;

public:
    Employee(const std::string &name, const std::string &position, double salary)
        : name_(name), position_(position), salary_(salary) {}

    virtual ~Employee() = default;

    virtual void showDetails(int level = 0) const = 0;
    virtual double getTotalSalary() const = 0;

    virtual void addSubordinate(std::shared_ptr<Employee> emp)
    {
        throw std::runtime_error("Cannot add subordinate to individual contributor");
    }
};

// Leaf: Individual contributor
class IndividualContributor : public Employee
{
public:
    IndividualContributor(const std::string &name, const std::string &position, double salary)
        : Employee(name, position, salary) {}

    void showDetails(int level = 0) const override
    {
        std::string indent(level * 2, ' ');
        std::cout << indent << "👤 " << name_ << " - " << position_
                  << " ($" << salary_ << ")\n";
    }

    double getTotalSalary() const override
    {
        return salary_;
    }
};

// Composite: Manager
class Manager : public Employee
{
private:
    std::vector<std::shared_ptr<Employee>> subordinates_;

public:
    Manager(const std::string &name, const std::string &position, double salary)
        : Employee(name, position, salary) {}

    void addSubordinate(std::shared_ptr<Employee> emp) override
    {
        subordinates_.push_back(emp);
    }

    void showDetails(int level = 0) const override
    {
        std::string indent(level * 2, ' ');
        std::cout << indent << "👔 " << name_ << " - " << position_
                  << " ($" << salary_ << ")\n";

        for (const auto &subordinate : subordinates_)
        {
            subordinate->showDetails(level + 1);
        }
    }

    double getTotalSalary() const override
    {
        double total = salary_;
        for (const auto &subordinate : subordinates_)
        {
            total += subordinate->getTotalSalary();
        }
        return total;
    }
};

// ============================================================================
// Demonstration
// ============================================================================

void demonstrateFileSystem()
{
    std::cout << "\n--- File System Example ---\n";

    // Build tree structure
    auto root = std::make_shared<Directory>("root");
    auto home = std::make_shared<Directory>("home");
    auto user = std::make_shared<Directory>("user");

    user->add(std::make_shared<File>("document.txt", 120));
    user->add(std::make_shared<File>("photo.jpg", 2048));

    auto projects = std::make_shared<Directory>("projects");
    projects->add(std::make_shared<File>("main.cpp", 45));
    projects->add(std::make_shared<File>("header.h", 12));

    user->add(projects);
    home->add(user);
    root->add(home);
    root->add(std::make_shared<File>("boot.bin", 512));

    // Uniform treatment
    root->display();
    std::cout << "\nTotal size: " << root->getSize() << " KB\n";
}

void demonstrateUI()
{
    std::cout << "\n--- UI Component Example ---\n";

    auto mainPanel = std::make_shared<Panel>("MainWindow");

    auto loginPanel = std::make_shared<Panel>("LoginPanel");
    loginPanel->add(std::make_shared<TextBox>("username"));
    loginPanel->add(std::make_shared<TextBox>("password"));
    loginPanel->add(std::make_shared<Button>("Login"));

    mainPanel->add(loginPanel);
    mainPanel->add(std::make_shared<Button>("Exit"));

    mainPanel->render();
    std::cout << "\nSimulating click event:\n";
    mainPanel->handleEvent("click");
}

void demonstrateOrganization()
{
    std::cout << "\n--- Organization Hierarchy Example ---\n";

    auto ceo = std::make_shared<Manager>("Alice", "CEO", 200000);

    auto cto = std::make_shared<Manager>("Bob", "CTO", 150000);
    cto->addSubordinate(std::make_shared<IndividualContributor>("Charlie", "Senior Dev", 120000));
    cto->addSubordinate(std::make_shared<IndividualContributor>("Diana", "Dev", 90000));

    auto cfo = std::make_shared<Manager>("Eve", "CFO", 150000);
    cfo->addSubordinate(std::make_shared<IndividualContributor>("Frank", "Accountant", 70000));

    ceo->addSubordinate(cto);
    ceo->addSubordinate(cfo);

    ceo->showDetails();
    std::cout << "\nTotal payroll: $" << ceo->getTotalSalary() << "\n";
}

int main()
{
    std::cout << "=== COMPOSITE PATTERN DEMO ===\n";

    demonstrateFileSystem();
    demonstrateUI();
    demonstrateOrganization();

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Uniform interface for leaf and composite objects\n";
    std::cout << "2. Tree structures represent part-whole hierarchies\n";
    std::cout << "3. Client treats individuals and compositions identically\n";
    std::cout << "4. Recursive composition enables arbitrary depth\n";
    std::cout << "5. Operations propagate through the tree naturally\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Represent part-whole hierarchies
 * - Want clients to treat individual and composite objects uniformly
 * - Tree-like structure where nodes can be leaves or branches
 * - Operations should work recursively across the structure
 *
 * BENEFITS:
 * - Simplifies client code (no type checking for leaf vs composite)
 * - Easy to add new component types (OCP)
 * - Recursive operations come naturally
 * - Flexibility in structure composition
 *
 * DRAWBACKS:
 * - Can make design overly general
 * - Hard to restrict component types in composite
 * - Interface pollution (leaf may not support all operations)
 *
 * DESIGN CONSIDERATIONS:
 * - Should Component declare child management operations? (transparency vs safety)
 * - Transparent: All in Component (easier for client, but leaf gets irrelevant methods)
 * - Safe: Only in Composite (client must check type, loses uniformity)
 * - Parent references? Useful for navigation but adds complexity
 * - Ordering of children? May need iterator pattern
 * - Caching results? Performance optimization for expensive operations
 *
 * COMPARISON:
 * - Composite: Part-whole hierarchy, uniform treatment
 * - Decorator: Adds responsibility, linear chain not tree
 * - Chain of Responsibility: Linear chain, request handling
 */
