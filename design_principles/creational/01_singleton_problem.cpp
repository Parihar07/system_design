/**
 * SINGLETON PATTERN - Problem Demonstration
 *
 * Intent: Ensure a class has only one instance and provide global access to it.
 *
 * When to Use:
 * - Need exactly one instance of a class
 * - Global access point needed
 * - Instance must be extensible by subclassing
 * - Lazy initialization required
 *
 * Common Use Cases:
 * - Logger, Configuration Manager, Database Connection Pool
 * - Thread Pool, Cache Manager, File System
 * - Device drivers, Print spoolers
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ============================================================================
// PROBLEM 1: Multiple instances can be created
// ============================================================================

namespace problem_multiple_instances
{
    class Logger
    {
    private:
        string logFile;
        vector<string> logs;

    public:
        Logger(const string &file) : logFile(file)
        {
            cout << "Logger created for: " << logFile << "\n";
        }

        void log(const string &message)
        {
            logs.push_back(message);
            cout << "[" << logFile << "] " << message << "\n";
        }

        void showStats()
        {
            cout << "Total logs: " << logs.size() << "\n";
        }
    };

    void demonstrateProblem()
    {
        cout << "=== PROBLEM: Multiple Logger Instances ===\n";

        // Problem: We can create multiple loggers
        Logger logger1("app.log");
        Logger logger2("app.log"); // Oops! Another instance for same file
        Logger logger3("app.log"); // And another!

        logger1.log("User logged in");
        logger2.log("Data processed");
        logger3.log("Request completed");

        // Each logger has its own state - logs are not shared!
        cout << "\nLogger1 stats: ";
        logger1.showStats(); // Shows 1 log

        cout << "Logger2 stats: ";
        logger2.showStats(); // Shows 1 log

        cout << "Logger3 stats: ";
        logger3.showStats(); // Shows 1 log

        cout << "\nProblem: We have 3 instances when we need only 1!\n";
        cout << "Each has incomplete view of all logs.\n";
    }
}

// ============================================================================
// PROBLEM 2: Resource contention and synchronization issues
// ============================================================================

namespace problem_resource_contention
{
    class DatabaseConnection
    {
    private:
        string connectionString;
        bool connected;

    public:
        DatabaseConnection(const string &connStr) : connectionString(connStr), connected(false)
        {
            cout << "Opening database connection: " << connStr << "\n";
            // Expensive operation: opening socket, authentication, etc.
            connected = true;
        }

        ~DatabaseConnection()
        {
            if (connected)
            {
                cout << "Closing database connection\n";
            }
        }

        void executeQuery(const string &query)
        {
            if (connected)
            {
                cout << "Executing: " << query << "\n";
            }
        }
    };

    void demonstrateProblem()
    {
        cout << "\n=== PROBLEM: Multiple Database Connections ===\n";

        // Problem: Each component creates its own connection
        DatabaseConnection conn1("localhost:5432/mydb");
        conn1.executeQuery("SELECT * FROM users");

        DatabaseConnection conn2("localhost:5432/mydb");
        conn2.executeQuery("SELECT * FROM orders");

        DatabaseConnection conn3("localhost:5432/mydb");
        conn3.executeQuery("SELECT * FROM products");

        cout << "\nProblem: 3 connections opened to same database!\n";
        cout << "- Wastes resources (connections, memory)\n";
        cout << "- Potential for connection pool exhaustion\n";
        cout << "- Synchronization issues if connections interfere\n";
    }
}

// ============================================================================
// PROBLEM 3: Configuration inconsistency
// ============================================================================

namespace problem_configuration
{
    class AppConfig
    {
    private:
        string environment;
        int maxConnections;
        bool debugMode;

    public:
        AppConfig()
        {
            cout << "Loading configuration from file...\n";
            environment = "production";
            maxConnections = 100;
            debugMode = false;
        }

        void setEnvironment(const string &env) { environment = env; }
        void setMaxConnections(int max) { maxConnections = max; }
        void setDebugMode(bool debug) { debugMode = debug; }

        void display()
        {
            cout << "Config - Env: " << environment
                 << ", MaxConn: " << maxConnections
                 << ", Debug: " << (debugMode ? "ON" : "OFF") << "\n";
        }
    };

    void demonstrateProblem()
    {
        cout << "\n=== PROBLEM: Inconsistent Configuration ===\n";

        // Problem: Different parts of app have different configs
        AppConfig config1;
        config1.setEnvironment("development");
        config1.setDebugMode(true);

        AppConfig config2; // Creates new instance with defaults
        config2.setMaxConnections(50);

        cout << "\nConfig in Module A: ";
        config1.display();

        cout << "Config in Module B: ";
        config2.display();

        cout << "\nProblem: Inconsistent state across the application!\n";
        cout << "Module A thinks debug is ON, Module B thinks it's OFF.\n";
    }
}

// ============================================================================
// PROBLEM 4: Cannot prevent instantiation
// ============================================================================

namespace problem_no_control
{
    class PrintSpooler
    {
    private:
        vector<string> printQueue;
        bool printing;

    public:
        PrintSpooler() : printing(false)
        {
            cout << "Print Spooler initialized\n";
        }

        void addJob(const string &document)
        {
            printQueue.push_back(document);
            cout << "Added to queue: " << document << "\n";
        }

        void process()
        {
            if (!printQueue.empty())
            {
                cout << "Processing " << printQueue.size() << " print jobs\n";
                printQueue.clear();
            }
        }

        int queueSize() const { return printQueue.size(); }
    };

    void demonstrateProblem()
    {
        cout << "\n=== PROBLEM: No Control Over Instantiation ===\n";

        // Problem: Anyone can create a spooler
        PrintSpooler spooler1;
        spooler1.addJob("Document1.pdf");
        spooler1.addJob("Document2.pdf");

        PrintSpooler spooler2; // Oops! Created another spooler
        spooler2.addJob("Document3.pdf");

        cout << "\nSpooler1 queue size: " << spooler1.queueSize() << "\n";
        cout << "Spooler2 queue size: " << spooler2.queueSize() << "\n";

        cout << "\nProblem: Multiple spoolers processing different queues!\n";
        cout << "Print jobs are scattered across instances.\n";
        cout << "No centralized control over printing.\n";
    }
}

// ============================================================================
// PROBLEM 5: Global state with global variables (anti-pattern)
// ============================================================================

namespace problem_global_variable
{
    // Bad approach: Using global variable
    class CacheManager
    {
    public:
        void put(const string &key, const string &value)
        {
            cout << "Cached: " << key << " = " << value << "\n";
        }

        string get(const string &key)
        {
            return "value_for_" + key;
        }
    };

    // Global variable - problems:
    // 1. Initialization order fiasco (undefined initialization order across translation units)
    // 2. Cannot control when it's created
    // 3. Not thread-safe
    // 4. Difficult to test (cannot replace with mock)
    // 5. Tight coupling to concrete implementation
    CacheManager globalCache;

    void demonstrateProblem()
    {
        cout << "\n=== PROBLEM: Global Variable Approach ===\n";

        globalCache.put("user:1", "John");
        string value = globalCache.get("user:1");

        cout << "Retrieved: " << value << "\n";

        cout << "\nProblems with global variables:\n";
        cout << "1. Static initialization order fiasco\n";
        cout << "2. Cannot control construction/destruction\n";
        cout << "3. Not thread-safe by default\n";
        cout << "4. Tight coupling - hard to mock for testing\n";
        cout << "5. Pollutes global namespace\n";
        cout << "6. Implicit dependencies (hard to track usage)\n";
    }
}

// ============================================================================
// KEY PROBLEMS SUMMARY
// ============================================================================

void printProblemsSummary()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "WHY SINGLETON PATTERN IS NEEDED - PROBLEMS WITHOUT IT:\n";
    cout << string(70, '=') << "\n\n";

    cout << "1. MULTIPLE INSTANCES:\n";
    cout << "   - Resources duplicated unnecessarily\n";
    cout << "   - Inconsistent state across instances\n";
    cout << "   - Wasted memory and system resources\n\n";

    cout << "2. RESOURCE CONTENTION:\n";
    cout << "   - Multiple connections to same resource\n";
    cout << "   - Connection pool exhaustion\n";
    cout << "   - Synchronization issues\n\n";

    cout << "3. CONFIGURATION INCONSISTENCY:\n";
    cout << "   - Different parts of app see different config\n";
    cout << "   - Hard to maintain consistent global state\n";
    cout << "   - Debugging becomes difficult\n\n";

    cout << "4. NO INSTANTIATION CONTROL:\n";
    cout << "   - Cannot enforce single instance constraint\n";
    cout << "   - Public constructors allow uncontrolled creation\n";
    cout << "   - No lazy initialization\n\n";

    cout << "5. GLOBAL VARIABLE ISSUES:\n";
    cout << "   - Static initialization order fiasco\n";
    cout << "   - Not thread-safe\n";
    cout << "   - Hard to test and mock\n";
    cout << "   - Tight coupling\n\n";

    cout << "SINGLETON SOLUTION:\n";
    cout << "- Controlled instantiation (private constructor)\n";
    cout << "- Single global access point\n";
    cout << "- Lazy initialization (created when first needed)\n";
    cout << "- Thread-safe instance creation\n";
    cout << "- Testable (can be mocked via interface)\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "SINGLETON PATTERN - PROBLEM DEMONSTRATION\n";
    cout << string(70, '=') << "\n\n";

    problem_multiple_instances::demonstrateProblem();
    problem_resource_contention::demonstrateProblem();
    problem_configuration::demonstrateProblem();
    problem_no_control::demonstrateProblem();
    problem_global_variable::demonstrateProblem();

    printProblemsSummary();

    return 0;
}
