/**
 * FACTORY METHOD PATTERN
 *
 * Intent: Define an interface for creating objects, but let subclasses decide
 *         which class to instantiate. Factory Method lets a class defer
 *         instantiation to subclasses.
 *
 * Also Known As: Virtual Constructor
 *
 * When to Use:
 * - A class can't anticipate the class of objects it must create
 * - A class wants its subclasses to specify the objects it creates
 * - Classes delegate responsibility to helper subclasses
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ============================================================================
// PROBLEM: Tight coupling with concrete classes
// ============================================================================

namespace problem_tight_coupling
{
    class Button
    {
    public:
        virtual void render() = 0;
        virtual ~Button() = default;
    };

    class WindowsButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Windows] Rendering Windows-style button\n";
        }
    };

    class MacButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Mac] Rendering Mac-style button\n";
        }
    };

    // Problem: Client code is tightly coupled to concrete classes
    class Application
    {
    private:
        string platform;

    public:
        Application(const string &p) : platform(p) {}

        void createUI()
        {
            Button *button = nullptr;

            // Problem: Must modify this code for every new platform
            if (platform == "Windows")
            {
                button = new WindowsButton();
            }
            else if (platform == "Mac")
            {
                button = new MacButton();
            }
            // What about Linux? Web? Mobile?
            // Must keep adding if-else branches!

            if (button)
            {
                button->render();
                delete button;
            }
        }
    };

    void demonstrate()
    {
        cout << "=== PROBLEM: Tight Coupling ===\n";
        Application winApp("Windows");
        winApp.createUI();

        Application macApp("Mac");
        macApp.createUI();

        cout << "\nProblems:\n";
        cout << "- Must modify Application for every new platform\n";
        cout << "- Violates Open/Closed Principle\n";
        cout << "- if-else ladder grows with each platform\n";
    }
}

// ============================================================================
// SOLUTION: Factory Method Pattern
// ============================================================================

namespace factory_method_solution
{
    // Product interface
    class Button
    {
    public:
        virtual void render() = 0;
        virtual void onClick() = 0;
        virtual ~Button() = default;
    };

    // Concrete Products
    class WindowsButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Windows] █▓▒░ Windows Button ░▒▓█\n";
        }

        void onClick() override
        {
            cout << "[Windows] Click sound: *ding*\n";
        }
    };

    class MacButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Mac] ◉ Mac Button ◉\n";
        }

        void onClick() override
        {
            cout << "[Mac] Click sound: *pop*\n";
        }
    };

    class LinuxButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Linux] [=] Linux Button [=]\n";
        }

        void onClick() override
        {
            cout << "[Linux] Click sound: *beep*\n";
        }
    };

    // Creator (Abstract Factory)
    class Dialog
    {
    public:
        virtual ~Dialog() = default;

        // Factory Method - deferred to subclasses
        virtual unique_ptr<Button> createButton() = 0;

        // Template method using factory method
        void render()
        {
            auto button = createButton();
            button->render();
        }

        void handleClick()
        {
            auto button = createButton();
            button->onClick();
        }
    };

    // Concrete Creators
    class WindowsDialog : public Dialog
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<WindowsButton>();
        }
    };

    class MacDialog : public Dialog
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<MacButton>();
        }
    };

    class LinuxDialog : public Dialog
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<LinuxButton>();
        }
    };

    void demonstrate()
    {
        cout << "\n=== SOLUTION: Factory Method Pattern ===\n";

        vector<unique_ptr<Dialog>> dialogs;
        dialogs.push_back(make_unique<WindowsDialog>());
        dialogs.push_back(make_unique<MacDialog>());
        dialogs.push_back(make_unique<LinuxDialog>());

        for (auto &dialog : dialogs)
        {
            dialog->render();
            dialog->handleClick();
            cout << "\n";
        }

        cout << "Benefits:\n";
        cout << "✓ No if-else ladder in client code\n";
        cout << "✓ Easy to add new platforms (just add new creator)\n";
        cout << "✓ Follows Open/Closed Principle\n";
        cout << "✓ Single Responsibility - creation logic separated\n";
    }
}

// ============================================================================
// REAL-WORLD EXAMPLE: Document Editor
// ============================================================================

namespace document_editor
{
    // Product interface
    class Document
    {
    public:
        virtual ~Document() = default;
        virtual void open() = 0;
        virtual void save() = 0;
        virtual string getType() const = 0;
    };

    // Concrete Products
    class PDFDocument : public Document
    {
    public:
        void open() override
        {
            cout << "Opening PDF document with PDF reader\n";
        }

        void save() override
        {
            cout << "Saving as PDF format\n";
        }

        string getType() const override
        {
            return "PDF";
        }
    };

    class WordDocument : public Document
    {
    public:
        void open() override
        {
            cout << "Opening Word document with text editor\n";
        }

        void save() override
        {
            cout << "Saving as DOCX format\n";
        }

        string getType() const override
        {
            return "Word";
        }
    };

    class SpreadsheetDocument : public Document
    {
    public:
        void open() override
        {
            cout << "Opening spreadsheet with grid view\n";
        }

        void save() override
        {
            cout << "Saving as XLSX format\n";
        }

        string getType() const override
        {
            return "Spreadsheet";
        }
    };

    // Creator
    class Application
    {
    public:
        virtual ~Application() = default;
        virtual unique_ptr<Document> createDocument() = 0;

        void newDocument()
        {
            auto doc = createDocument();
            cout << "Created new " << doc->getType() << " document\n";
            doc->open();
        }
    };

    // Concrete Creators
    class PDFApplication : public Application
    {
    public:
        unique_ptr<Document> createDocument() override
        {
            return make_unique<PDFDocument>();
        }
    };

    class WordApplication : public Application
    {
    public:
        unique_ptr<Document> createDocument() override
        {
            return make_unique<WordDocument>();
        }
    };

    class SpreadsheetApplication : public Application
    {
    public:
        unique_ptr<Document> createDocument() override
        {
            return make_unique<SpreadsheetDocument>();
        }
    };

    void demonstrate()
    {
        cout << "\n=== DOCUMENT EDITOR EXAMPLE ===\n";

        vector<unique_ptr<Application>> apps;
        apps.push_back(make_unique<PDFApplication>());
        apps.push_back(make_unique<WordApplication>());
        apps.push_back(make_unique<SpreadsheetApplication>());

        for (auto &app : apps)
        {
            app->newDocument();
            cout << "\n";
        }
    }
}

// ============================================================================
// COMPARISON AND GUIDELINES
// ============================================================================

void printGuidelines()
{
    cout << string(70, '=') << "\n";
    cout << "FACTORY METHOD PATTERN - KEY POINTS\n";
    cout << string(70, '=') << "\n\n";

    cout << "STRUCTURE:\n";
    cout << "  Product (interface) ← ConcreteProduct (implementation)\n";
    cout << "  Creator (abstract) ← ConcreteCreator (factory)\n";
    cout << "  Creator::factoryMethod() returns Product\n\n";

    cout << "PARTICIPANTS:\n";
    cout << "  • Product: Defines interface of objects factory creates\n";
    cout << "  • ConcreteProduct: Implements Product interface\n";
    cout << "  • Creator: Declares factory method\n";
    cout << "  • ConcreteCreator: Overrides factory method\n\n";

    cout << "WHEN TO USE:\n";
    cout << "  ✓ Don't know exact types beforehand\n";
    cout << "  ✓ Want to provide extension points\n";
    cout << "  ✓ Want to save system resources (reuse objects)\n";
    cout << "  ✓ Need to delegate instantiation to subclasses\n\n";

    cout << "BENEFITS:\n";
    cout << "  • Loose coupling between creator and products\n";
    cout << "  • Single Responsibility Principle\n";
    cout << "  • Open/Closed Principle\n";
    cout << "  • Flexibility in choosing products\n\n";

    cout << "DRAWBACKS:\n";
    cout << "  • Can become complex with many subclasses\n";
    cout << "  • Requires subclassing to extend\n\n";

    cout << "RELATED PATTERNS:\n";
    cout << "  • Abstract Factory: Uses factory methods\n";
    cout << "  • Template Method: Often uses factory methods\n";
    cout << "  • Prototype: Alternative to factory method\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "FACTORY METHOD PATTERN\n";
    cout << string(70, '=') << "\n";

    problem_tight_coupling::demonstrate();
    factory_method_solution::demonstrate();
    document_editor::demonstrate();

    printGuidelines();

    return 0;
}
