/**
 * ABSTRACT FACTORY PATTERN
 *
 * Intent: Provide an interface for creating families of related or dependent
 *         objects without specifying their concrete classes.
 *
 * Key Difference from Factory Method:
 * - Factory Method: Creates ONE type of product (single product hierarchy)
 * - Abstract Factory: Creates FAMILIES of related products (multiple product hierarchies)
 *
 * When to Use:
 * - System needs to be independent of how its products are created
 * - System configured with one of multiple families of products
 * - Family of related products designed to be used together
 * - Want to reveal only interfaces, not implementations
 *
 * Common Use Cases:
 * - Cross-platform UI toolkits (Windows/Mac/Linux widgets)
 * - Database access layers (MySQL/PostgreSQL/MongoDB clients)
 * - Document converters (PDF/Word/HTML generators)
 * - Theme systems (Dark/Light UI components)
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ============================================================================
// PROBLEM: Mixing incompatible product families
// ============================================================================

namespace problem_mixed_families
{
    // Different button styles
    class Button
    {
    public:
        virtual void render() = 0;
        virtual ~Button() = default;
    };

    class WindowsButton : public Button
    {
    public:
        void render() override { cout << "[Windows] ▭ Button\n"; }
    };

    class MacButton : public Button
    {
    public:
        void render() override { cout << "[Mac] ◉ Button\n"; }
    };

    // Different checkbox styles
    class Checkbox
    {
    public:
        virtual void render() = 0;
        virtual ~Checkbox() = default;
    };

    class WindowsCheckbox : public Checkbox
    {
    public:
        void render() override { cout << "[Windows] ☑ Checkbox\n"; }
    };

    class MacCheckbox : public Checkbox
    {
    public:
        void render() override { cout << "[Mac] ✓ Checkbox\n"; }
    };

    // Problem: Client code can mix incompatible widgets
    class Application
    {
    public:
        void createUI(const string &platform)
        {
            Button *button;
            Checkbox *checkbox;

            // Problem: Easy to accidentally mix platforms!
            if (platform == "Windows")
            {
                button = new WindowsButton();
                checkbox = new MacCheckbox(); // Oops! Mixed platforms
            }
            else
            {
                button = new MacButton();
                checkbox = new WindowsCheckbox(); // Oops! Mixed again
            }

            button->render();
            checkbox->render();

            delete button;
            delete checkbox;
        }
    };

    void demonstrate()
    {
        cout << "=== PROBLEM: Mixing Product Families ===\n";
        Application app;
        app.createUI("Windows");

        cout << "\nProblem: Windows button with Mac checkbox!\n";
        cout << "UI looks inconsistent - widgets from different families\n";
    }
}

// ============================================================================
// SOLUTION: Abstract Factory Pattern
// ============================================================================

namespace abstract_factory_solution
{
    // Abstract Products
    class Button
    {
    public:
        virtual ~Button() = default;
        virtual void render() = 0;
        virtual void onClick() = 0;
    };

    class Checkbox
    {
    public:
        virtual ~Checkbox() = default;
        virtual void render() = 0;
        virtual void toggle() = 0;
    };

    class TextField
    {
    public:
        virtual ~TextField() = default;
        virtual void render() = 0;
        virtual void setText(const string &text) = 0;
    };

    // Windows Family
    class WindowsButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Windows] █▓▒░ Button ░▒▓█\n";
        }
        void onClick() override
        {
            cout << "[Windows] Button clicked with Windows animation\n";
        }
    };

    class WindowsCheckbox : public Checkbox
    {
    public:
        void render() override
        {
            cout << "[Windows] ☑ Checkbox\n";
        }
        void toggle() override
        {
            cout << "[Windows] Checkbox toggled\n";
        }
    };

    class WindowsTextField : public TextField
    {
    public:
        void render() override
        {
            cout << "[Windows] ▭▭▭▭▭ TextField\n";
        }
        void setText(const string &text) override
        {
            cout << "[Windows] Text set: " << text << "\n";
        }
    };

    // Mac Family
    class MacButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Mac] ◉ Button ◉\n";
        }
        void onClick() override
        {
            cout << "[Mac] Button clicked with smooth fade\n";
        }
    };

    class MacCheckbox : public Checkbox
    {
    public:
        void render() override
        {
            cout << "[Mac] ✓ Checkbox\n";
        }
        void toggle() override
        {
            cout << "[Mac] Checkbox animated toggle\n";
        }
    };

    class MacTextField : public TextField
    {
    public:
        void render() override
        {
            cout << "[Mac] ◯◯◯◯◯ TextField\n";
        }
        void setText(const string &text) override
        {
            cout << "[Mac] Text set: " << text << "\n";
        }
    };

    // Linux Family
    class LinuxButton : public Button
    {
    public:
        void render() override
        {
            cout << "[Linux] [=] Button\n";
        }
        void onClick() override
        {
            cout << "[Linux] Button clicked\n";
        }
    };

    class LinuxCheckbox : public Checkbox
    {
    public:
        void render() override
        {
            cout << "[Linux] [x] Checkbox\n";
        }
        void toggle() override
        {
            cout << "[Linux] Checkbox state changed\n";
        }
    };

    class LinuxTextField : public TextField
    {
    public:
        void render() override
        {
            cout << "[Linux] |___| TextField\n";
        }
        void setText(const string &text) override
        {
            cout << "[Linux] Text: " << text << "\n";
        }
    };

    // Abstract Factory Interface
    class GUIFactory
    {
    public:
        virtual ~GUIFactory() = default;
        virtual unique_ptr<Button> createButton() = 0;
        virtual unique_ptr<Checkbox> createCheckbox() = 0;
        virtual unique_ptr<TextField> createTextField() = 0;
    };

    // Concrete Factories
    class WindowsFactory : public GUIFactory
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<WindowsButton>();
        }
        unique_ptr<Checkbox> createCheckbox() override
        {
            return make_unique<WindowsCheckbox>();
        }
        unique_ptr<TextField> createTextField() override
        {
            return make_unique<WindowsTextField>();
        }
    };

    class MacFactory : public GUIFactory
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<MacButton>();
        }
        unique_ptr<Checkbox> createCheckbox() override
        {
            return make_unique<MacCheckbox>();
        }
        unique_ptr<TextField> createTextField() override
        {
            return make_unique<MacTextField>();
        }
    };

    class LinuxFactory : public GUIFactory
    {
    public:
        unique_ptr<Button> createButton() override
        {
            return make_unique<LinuxButton>();
        }
        unique_ptr<Checkbox> createCheckbox() override
        {
            return make_unique<LinuxCheckbox>();
        }
        unique_ptr<TextField> createTextField() override
        {
            return make_unique<LinuxTextField>();
        }
    };

    // Client code works with abstract factory
    class Application
    {
    private:
        unique_ptr<Button> button;
        unique_ptr<Checkbox> checkbox;
        unique_ptr<TextField> textField;

    public:
        Application(GUIFactory &factory)
        {
            button = factory.createButton();
            checkbox = factory.createCheckbox();
            textField = factory.createTextField();
        }

        void render()
        {
            button->render();
            checkbox->render();
            textField->render();
        }

        void interact()
        {
            button->onClick();
            checkbox->toggle();
            textField->setText("Hello World");
        }
    };

    void demonstrate()
    {
        cout << "\n=== SOLUTION: Abstract Factory Pattern ===\n";

        cout << "\n--- Windows Theme ---\n";
        WindowsFactory winFactory;
        Application winApp(winFactory);
        winApp.render();
        winApp.interact();

        cout << "\n--- Mac Theme ---\n";
        MacFactory macFactory;
        Application macApp(macFactory);
        macApp.render();
        macApp.interact();

        cout << "\n--- Linux Theme ---\n";
        LinuxFactory linuxFactory;
        Application linuxApp(linuxFactory);
        linuxApp.render();

        cout << "\nBenefits:\n";
        cout << "✓ All widgets from same family (consistent look)\n";
        cout << "✓ Easy to switch entire theme\n";
        cout << "✓ Can't accidentally mix incompatible widgets\n";
        cout << "✓ Adding new family = one new factory class\n";
    }
}

// ============================================================================
// REAL-WORLD EXAMPLE: Database Access Layer
// ============================================================================

namespace database_access_layer
{
    // Abstract Products
    class Connection
    {
    public:
        virtual ~Connection() = default;
        virtual void connect(const string &connectionString) = 0;
        virtual void disconnect() = 0;
        virtual string getType() const = 0;
    };

    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute(const string &sql) = 0;
        virtual string getDialect() const = 0;
    };

    class Transaction
    {
    public:
        virtual ~Transaction() = default;
        virtual void begin() = 0;
        virtual void commit() = 0;
        virtual void rollback() = 0;
    };

    // MySQL Family
    class MySQLConnection : public Connection
    {
    public:
        void connect(const string &connStr) override
        {
            cout << "[MySQL] Connected to: " << connStr << "\n";
        }
        void disconnect() override
        {
            cout << "[MySQL] Disconnected\n";
        }
        string getType() const override { return "MySQL"; }
    };

    class MySQLCommand : public Command
    {
    public:
        void execute(const string &sql) override
        {
            cout << "[MySQL] Executing: " << sql << "\n";
        }
        string getDialect() const override { return "MySQL SQL"; }
    };

    class MySQLTransaction : public Transaction
    {
    public:
        void begin() override { cout << "[MySQL] BEGIN TRANSACTION\n"; }
        void commit() override { cout << "[MySQL] COMMIT\n"; }
        void rollback() override { cout << "[MySQL] ROLLBACK\n"; }
    };

    // PostgreSQL Family
    class PostgreSQLConnection : public Connection
    {
    public:
        void connect(const string &connStr) override
        {
            cout << "[PostgreSQL] Connected to: " << connStr << "\n";
        }
        void disconnect() override
        {
            cout << "[PostgreSQL] Disconnected\n";
        }
        string getType() const override { return "PostgreSQL"; }
    };

    class PostgreSQLCommand : public Command
    {
    public:
        void execute(const string &sql) override
        {
            cout << "[PostgreSQL] Executing: " << sql << "\n";
        }
        string getDialect() const override { return "PostgreSQL SQL"; }
    };

    class PostgreSQLTransaction : public Transaction
    {
    public:
        void begin() override { cout << "[PostgreSQL] START TRANSACTION\n"; }
        void commit() override { cout << "[PostgreSQL] COMMIT\n"; }
        void rollback() override { cout << "[PostgreSQL] ROLLBACK\n"; }
    };

    // MongoDB Family (NoSQL)
    class MongoDBConnection : public Connection
    {
    public:
        void connect(const string &connStr) override
        {
            cout << "[MongoDB] Connected to: " << connStr << "\n";
        }
        void disconnect() override
        {
            cout << "[MongoDB] Disconnected\n";
        }
        string getType() const override { return "MongoDB"; }
    };

    class MongoDBCommand : public Command
    {
    public:
        void execute(const string &query) override
        {
            cout << "[MongoDB] Executing query: " << query << "\n";
        }
        string getDialect() const override { return "MongoDB Query Language"; }
    };

    class MongoDBTransaction : public Transaction
    {
    public:
        void begin() override { cout << "[MongoDB] Session started\n"; }
        void commit() override { cout << "[MongoDB] Session committed\n"; }
        void rollback() override { cout << "[MongoDB] Session aborted\n"; }
    };

    // Abstract Factory
    class DatabaseFactory
    {
    public:
        virtual ~DatabaseFactory() = default;
        virtual unique_ptr<Connection> createConnection() = 0;
        virtual unique_ptr<Command> createCommand() = 0;
        virtual unique_ptr<Transaction> createTransaction() = 0;
    };

    // Concrete Factories
    class MySQLFactory : public DatabaseFactory
    {
    public:
        unique_ptr<Connection> createConnection() override
        {
            return make_unique<MySQLConnection>();
        }
        unique_ptr<Command> createCommand() override
        {
            return make_unique<MySQLCommand>();
        }
        unique_ptr<Transaction> createTransaction() override
        {
            return make_unique<MySQLTransaction>();
        }
    };

    class PostgreSQLFactory : public DatabaseFactory
    {
    public:
        unique_ptr<Connection> createConnection() override
        {
            return make_unique<PostgreSQLConnection>();
        }
        unique_ptr<Command> createCommand() override
        {
            return make_unique<PostgreSQLCommand>();
        }
        unique_ptr<Transaction> createTransaction() override
        {
            return make_unique<PostgreSQLTransaction>();
        }
    };

    class MongoDBFactory : public DatabaseFactory
    {
    public:
        unique_ptr<Connection> createConnection() override
        {
            return make_unique<MongoDBConnection>();
        }
        unique_ptr<Command> createCommand() override
        {
            return make_unique<MongoDBCommand>();
        }
        unique_ptr<Transaction> createTransaction() override
        {
            return make_unique<MongoDBTransaction>();
        }
    };

    // Data Access Layer (Client)
    class DataAccessLayer
    {
    private:
        unique_ptr<Connection> connection;
        unique_ptr<Command> command;
        unique_ptr<Transaction> transaction;

    public:
        DataAccessLayer(DatabaseFactory &factory)
        {
            connection = factory.createConnection();
            command = factory.createCommand();
            transaction = factory.createTransaction();
        }

        void performDatabaseOperations(const string &connStr)
        {
            connection->connect(connStr);

            transaction->begin();
            command->execute("SELECT * FROM users WHERE active = 1");
            command->execute("UPDATE users SET last_login = NOW()");
            transaction->commit();

            connection->disconnect();
        }
    };

    void demonstrate()
    {
        cout << "\n=== DATABASE ACCESS LAYER EXAMPLE ===\n";

        cout << "\n--- Using MySQL ---\n";
        MySQLFactory mysqlFactory;
        DataAccessLayer mysqlDAL(mysqlFactory);
        mysqlDAL.performDatabaseOperations("localhost:3306/mydb");

        cout << "\n--- Using PostgreSQL ---\n";
        PostgreSQLFactory pgFactory;
        DataAccessLayer pgDAL(pgFactory);
        pgDAL.performDatabaseOperations("localhost:5432/mydb");

        cout << "\n--- Using MongoDB ---\n";
        MongoDBFactory mongoFactory;
        DataAccessLayer mongoDAL(mongoFactory);
        mongoDAL.performDatabaseOperations("mongodb://localhost:27017/mydb");

        cout << "\nKey Point: All database components work together!\n";
        cout << "Can't accidentally mix MySQL connection with MongoDB command.\n";
    }
}

// ============================================================================
// COMPARISON AND GUIDELINES
// ============================================================================

void printGuidelines()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "ABSTRACT FACTORY vs FACTORY METHOD\n";
    cout << string(70, '=') << "\n\n";

    cout << "FACTORY METHOD:\n";
    cout << "  • Creates ONE type of product\n";
    cout << "  • Single product hierarchy\n";
    cout << "  • Uses inheritance (subclasses override factory method)\n";
    cout << "  • Example: DocumentFactory creates different Documents\n\n";

    cout << "ABSTRACT FACTORY:\n";
    cout << "  • Creates FAMILIES of related products\n";
    cout << "  • Multiple product hierarchies\n";
    cout << "  • Uses composition (factory object creates products)\n";
    cout << "  • Example: GUIFactory creates Button+Checkbox+TextField\n\n";

    cout << "WHEN TO USE ABSTRACT FACTORY:\n";
    cout << "  ✓ Products must be used together (UI theme, DB components)\n";
    cout << "  ✓ Need to ensure product compatibility\n";
    cout << "  ✓ Want to switch entire product family at once\n";
    cout << "  ✓ Multiple related products with variants\n\n";

    cout << "STRUCTURE:\n";
    cout << "  AbstractFactory → ConcreteFactoryA, ConcreteFactoryB\n";
    cout << "      ↓                    ↓                 ↓\n";
    cout << "  createProductX()    ProductA1        ProductB1\n";
    cout << "  createProductY()    ProductA2        ProductB2\n\n";

    cout << "BENEFITS:\n";
    cout << "  • Consistency among products\n";
    cout << "  • Isolates concrete classes\n";
    cout << "  • Easy to exchange product families\n";
    cout << "  • Promotes consistency (products designed together)\n\n";

    cout << "DRAWBACKS:\n";
    cout << "  • Difficult to add new products (must change all factories)\n";
    cout << "  • More complex than Factory Method\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "ABSTRACT FACTORY PATTERN\n";
    cout << string(70, '=') << "\n";

    problem_mixed_families::demonstrate();
    abstract_factory_solution::demonstrate();
    database_access_layer::demonstrate();

    printGuidelines();

    cout << "\nKEY TAKEAWAY:\n";
    cout << "Abstract Factory creates FAMILIES of related objects\n";
    cout << "Factory Method creates SINGLE objects\n";

    return 0;
}
