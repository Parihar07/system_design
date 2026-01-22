/**
 * SINGLETON PATTERN - Solution Implementation
 *
 * Key Components:
 * 1. Private constructor - prevents direct instantiation
 * 2. Static instance method - provides global access point
 * 3. Static member variable - holds the single instance
 * 4. Delete copy constructor and assignment operator - prevent copying
 *
 * Thread Safety:
 * - C++11 guarantees thread-safe static local initialization
 * - Meyer's Singleton (static local variable) is the modern approach
 */

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <map>

using namespace std;

// ============================================================================
// BASIC SINGLETON IMPLEMENTATION (Meyer's Singleton - C++11)
// ============================================================================

namespace basic_singleton
{
    class Logger
    {
    private:
        string logFile;
        vector<string> logs;

        // Private constructor - cannot be called from outside
        Logger(const string &file = "app.log") : logFile(file)
        {
            cout << "Logger instance created for: " << logFile << "\n";
        }

        // Delete copy constructor and assignment operator
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

    public:
        // Static method provides global access point
        static Logger &getInstance()
        {
            // Static local variable - created only once
            // Thread-safe in C++11 and later (magic statics)
            static Logger instance("app.log");
            return instance;
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

        int getLogCount() const
        {
            return logs.size();
        }
    };

    void demonstrate()
    {
        cout << "=== BASIC SINGLETON SOLUTION ===\n";

        // Get the singleton instance
        Logger &logger1 = Logger::getInstance();
        logger1.log("User logged in");

        // Get the "same" instance
        Logger &logger2 = Logger::getInstance();
        logger2.log("Data processed");

        // Get again
        Logger &logger3 = Logger::getInstance();
        logger3.log("Request completed");

        // All references point to the same instance
        cout << "\nLogger1 stats: ";
        logger1.showStats(); // Shows 3 logs

        cout << "Logger2 stats: ";
        logger2.showStats(); // Shows 3 logs

        cout << "Logger3 stats: ";
        logger3.showStats(); // Shows 3 logs

        cout << "\nAll references share the same instance!\n";
        cout << "Address of logger1: " << &logger1 << "\n";
        cout << "Address of logger2: " << &logger2 << "\n";
        cout << "Address of logger3: " << &logger3 << "\n";
    }
}

// ============================================================================
// SINGLETON WITH PARAMETERS (Configuration Manager)
// ============================================================================

namespace configurable_singleton
{
    class DatabaseConnection
    {
    private:
        string connectionString;
        bool connected;
        int queryCount;

        DatabaseConnection() : connected(false), queryCount(0)
        {
            cout << "DatabaseConnection created (not yet connected)\n";
        }

        DatabaseConnection(const DatabaseConnection &) = delete;
        DatabaseConnection &operator=(const DatabaseConnection &) = delete;

    public:
        static DatabaseConnection &getInstance()
        {
            static DatabaseConnection instance;
            return instance;
        }

        // Initialize connection (can be called once after getInstance)
        void initialize(const string &connStr)
        {
            if (!connected)
            {
                connectionString = connStr;
                cout << "Connecting to: " << connectionString << "\n";
                connected = true;
            }
            else
            {
                cout << "Already connected to: " << connectionString << "\n";
            }
        }

        void executeQuery(const string &query)
        {
            if (connected)
            {
                queryCount++;
                cout << "Query #" << queryCount << ": " << query << "\n";
            }
            else
            {
                cout << "Error: Not connected!\n";
            }
        }

        void showStats()
        {
            cout << "Connection: " << connectionString
                 << ", Queries executed: " << queryCount << "\n";
        }
    };

    void demonstrate()
    {
        cout << "\n=== CONFIGURABLE SINGLETON ===\n";

        DatabaseConnection &conn = DatabaseConnection::getInstance();
        conn.initialize("localhost:5432/mydb");

        conn.executeQuery("SELECT * FROM users");
        conn.executeQuery("SELECT * FROM orders");

        // Getting instance from another part of code
        DatabaseConnection &conn2 = DatabaseConnection::getInstance();
        conn2.executeQuery("SELECT * FROM products");

        conn.showStats();
        cout << "Same instance used across application!\n";
    }
}

// ============================================================================
// THREAD-SAFE SINGLETON WITH EXPLICIT LOCKING (Legacy approach)
// ============================================================================

namespace thread_safe_singleton
{
    class CacheManager
    {
    private:
        map<string, string> cache;
        mutable mutex cacheMutex; // Protects cache access
        static CacheManager *instance;
        static mutex instanceMutex; // Protects instance creation

        CacheManager()
        {
            cout << "CacheManager instance created\n";
        }

        CacheManager(const CacheManager &) = delete;
        CacheManager &operator=(const CacheManager &) = delete;

    public:
        // Double-checked locking pattern (for demonstration)
        // Note: In modern C++, use Meyer's Singleton instead
        static CacheManager *getInstance()
        {
            if (instance == nullptr)
            {
                lock_guard<mutex> lock(instanceMutex);
                if (instance == nullptr)
                {
                    instance = new CacheManager();
                }
            }
            return instance;
        }

        void put(const string &key, const string &value)
        {
            lock_guard<mutex> lock(cacheMutex);
            cache[key] = value;
            cout << "Cached: " << key << " = " << value << "\n";
        }

        string get(const string &key)
        {
            lock_guard<mutex> lock(cacheMutex);
            if (cache.find(key) != cache.end())
            {
                return cache[key];
            }
            return "";
        }

        int size() const
        {
            lock_guard<mutex> lock(cacheMutex);
            return cache.size();
        }
    };

    // Static member initialization
    CacheManager *CacheManager::instance = nullptr;
    mutex CacheManager::instanceMutex;

    void demonstrate()
    {
        cout << "\n=== THREAD-SAFE SINGLETON (Legacy) ===\n";
        cout << "Note: This uses explicit locking for demonstration.\n";
        cout << "Modern C++ prefers Meyer's Singleton (static local variable).\n\n";

        CacheManager *cache = CacheManager::getInstance();
        cache->put("user:1", "John");
        cache->put("user:2", "Jane");

        string value = cache->get("user:1");
        cout << "Retrieved: " << value << "\n";

        cout << "Cache size: " << cache->size() << "\n";
    }
}

// ============================================================================
// SINGLETON WITH INTERFACE (For Testing and Dependency Injection)
// ============================================================================

namespace testable_singleton
{
    // Interface for logger
    class ILogger
    {
    public:
        virtual ~ILogger() = default;
        virtual void log(const string &message) = 0;
        virtual int getLogCount() const = 0;
    };

    // Concrete implementation
    class Logger : public ILogger
    {
    private:
        vector<string> logs;

        Logger()
        {
            cout << "Logger created\n";
        }

        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

    public:
        static Logger &getInstance()
        {
            static Logger instance;
            return instance;
        }

        void log(const string &message) override
        {
            logs.push_back(message);
            cout << "[LOG] " << message << "\n";
        }

        int getLogCount() const override
        {
            return logs.size();
        }
    };

    // Mock logger for testing
    class MockLogger : public ILogger
    {
    private:
        int logCount;

    public:
        MockLogger() : logCount(0)
        {
            cout << "MockLogger created for testing\n";
        }

        void log(const string &message) override
        {
            logCount++;
            cout << "[MOCK] " << message << "\n";
        }

        int getLogCount() const override
        {
            return logCount;
        }
    };

    // Service that depends on logger
    class UserService
    {
    private:
        ILogger &logger;

    public:
        UserService(ILogger &log) : logger(log) {}

        void registerUser(const string &username)
        {
            logger.log("Registering user: " + username);
            // Registration logic...
        }

        void loginUser(const string &username)
        {
            logger.log("User logged in: " + username);
            // Login logic...
        }
    };

    void demonstrate()
    {
        cout << "\n=== TESTABLE SINGLETON WITH INTERFACE ===\n";

        // Production code uses real singleton
        ILogger &logger = Logger::getInstance();
        UserService service(logger);
        service.registerUser("john_doe");
        service.loginUser("john_doe");

        cout << "\nTotal logs: " << logger.getLogCount() << "\n";

        // For testing, we can inject a mock
        cout << "\n--- Using Mock for Testing ---\n";
        MockLogger mockLogger;
        UserService testService(mockLogger);
        testService.registerUser("test_user");

        cout << "Mock logs: " << mockLogger.getLogCount() << "\n";
    }
}

// ============================================================================
// COMPARISON: Before vs After
// ============================================================================

void printComparison()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "BEFORE vs AFTER SINGLETON PATTERN\n";
    cout << string(70, '=') << "\n\n";

    cout << "WITHOUT SINGLETON:\n";
    cout << "  ❌ Multiple instances created\n";
    cout << "  ❌ Inconsistent state across instances\n";
    cout << "  ❌ Resource waste\n";
    cout << "  ❌ No control over instantiation\n";
    cout << "  ❌ Global variable issues\n\n";

    cout << "WITH SINGLETON:\n";
    cout << "  ✓ Single instance guaranteed\n";
    cout << "  ✓ Consistent global state\n";
    cout << "  ✓ Controlled access point\n";
    cout << "  ✓ Lazy initialization\n";
    cout << "  ✓ Thread-safe (C++11+)\n";
    cout << "  ✓ Testable (with interface)\n\n";

    cout << "WHEN TO USE:\n";
    cout << "  • Logger, Configuration Manager\n";
    cout << "  • Database Connection Pool\n";
    cout << "  • Thread Pool, Cache Manager\n";
    cout << "  • Hardware interface access\n";
    cout << "  • Application-wide registry\n\n";

    cout << "WHEN NOT TO USE:\n";
    cout << "  • If you need multiple instances later\n";
    cout << "  • Pure data containers (use dependency injection)\n";
    cout << "  • When testing requires different instances\n";
    cout << "  • Short-lived objects\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "SINGLETON PATTERN - SOLUTION IMPLEMENTATIONS\n";
    cout << string(70, '=') << "\n\n";

    basic_singleton::demonstrate();
    configurable_singleton::demonstrate();
    thread_safe_singleton::demonstrate();
    testable_singleton::demonstrate();

    printComparison();

    cout << "\nKEY TAKEAWAYS:\n";
    cout << "1. Use Meyer's Singleton (static local) in modern C++\n";
    cout << "2. Delete copy constructor and assignment operator\n";
    cout << "3. Make constructor private\n";
    cout << "4. Use interface for testability\n";
    cout << "5. Thread-safe by default in C++11+\n";

    return 0;
}
