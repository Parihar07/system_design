#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 * SPECIAL CONSTRUCTOR CASES
 * ==========================
 * Advanced constructor techniques that are frequently asked in interviews:
 * 1. Explicit Constructors (prevent implicit conversions)
 * 2. Private Constructors (singleton pattern, factory pattern)
 * 3. Delegating Constructors (C++11 - constructor calling another constructor)
 * 4. Delete/Default Keywords (C++11)
 */

// ============================================================================
// EXAMPLE 1: EXPLICIT KEYWORD - PREVENT IMPLICIT CONVERSIONS
// ============================================================================

class Distance {
private:
    int meters;

public:
    // WITHOUT explicit - allows implicit conversion
    Distance(int m) : meters(m) {
        cout << "Distance created: " << meters << " meters" << endl;
    }

    void display() {
        cout << "Distance: " << meters << " meters" << endl;
    }
};

class DistanceExplicit {
private:
    int meters;

public:
    // WITH explicit - prevents implicit conversion
    explicit DistanceExplicit(int m) : meters(m) {
        cout << "DistanceExplicit created: " << meters << " meters" << endl;
    }

    void display() {
        cout << "Distance: " << meters << " meters" << endl;
    }
};

void printDistance(Distance d) {
    d.display();
}

void printDistanceExplicit(DistanceExplicit d) {
    d.display();
}

// ============================================================================
// EXAMPLE 2: PRIVATE CONSTRUCTOR - SINGLETON PATTERN
// ============================================================================

class DatabaseConnection {
private:
    static DatabaseConnection* instance;
    string connectionString;

    // Private constructor - cannot create objects directly
    DatabaseConnection(string conn) : connectionString(conn) {
        cout << "Database connection established: " << conn << endl;
    }

public:
    // Delete copy constructor and assignment operator
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // Static method to get the single instance
    static DatabaseConnection* getInstance() {
        if (instance == nullptr) {
            instance = new DatabaseConnection("localhost:5432");
        }
        return instance;
    }

    void executeQuery(string query) {
        cout << "Executing query: " << query << endl;
    }

    ~DatabaseConnection() {
        cout << "Database connection closed" << endl;
    }
};

// Initialize static member
DatabaseConnection* DatabaseConnection::instance = nullptr;

// ============================================================================
// EXAMPLE 3: PRIVATE CONSTRUCTOR - FACTORY PATTERN
// ============================================================================

class Product {
private:
    string name;
    double price;
    string category;

    // Private constructor
    Product(string n, double p, string cat) 
        : name(n), price(p), category(cat) {
    }

public:
    // Factory methods - controlled object creation
    static Product createElectronics(string name, double price) {
        cout << "Creating Electronics product..." << endl;
        return Product(name, price, "Electronics");
    }

    static Product createClothing(string name, double price) {
        cout << "Creating Clothing product..." << endl;
        return Product(name, price, "Clothing");
    }

    static Product createFood(string name, double price) {
        cout << "Creating Food product..." << endl;
        return Product(name, price, "Food");
    }

    void display() {
        cout << "Product: " << name;
        cout << ", Price: ₹" << price;
        cout << ", Category: " << category << endl;
    }
};

// ============================================================================
// EXAMPLE 4: DELEGATING CONSTRUCTORS (C++11)
// ============================================================================

class Employee {
private:
    string name;
    int age;
    double salary;
    string department;

public:
    // Master constructor
    Employee(string n, int a, double s, string dept) 
        : name(n), age(a), salary(s), department(dept) {
        cout << "Master constructor called" << endl;
        // Common initialization logic here
        validateAge();
        validateSalary();
    }

    // Delegating constructor - calls master constructor
    Employee(string n, int a) 
        : Employee(n, a, 30000.0, "General") {  // Delegates to master
        cout << "Delegating constructor 1 called" << endl;
    }

    // Another delegating constructor
    Employee(string n) 
        : Employee(n, 25) {  // Delegates to above constructor
        cout << "Delegating constructor 2 called" << endl;
    }

    // Default constructor also delegates
    Employee() 
        : Employee("Unknown", 18, 25000.0, "Trainee") {
        cout << "Default constructor called" << endl;
    }

private:
    void validateAge() {
        if (age < 18 || age > 65) {
            cout << "Warning: Age out of normal range" << endl;
        }
    }

    void validateSalary() {
        if (salary < 20000) {
            cout << "Warning: Salary below minimum wage" << endl;
        }
    }

public:
    void display() {
        cout << "Employee: " << name;
        cout << ", Age: " << age;
        cout << ", Salary: ₹" << salary;
        cout << ", Dept: " << department << endl;
    }
};

// ============================================================================
// EXAMPLE 5: DELETE AND DEFAULT KEYWORDS (C++11)
// ============================================================================

class NonCopyable {
private:
    int* data;

public:
    NonCopyable(int val) {
        data = new int(val);
        cout << "NonCopyable created with value: " << val << endl;
    }

    // Delete copy constructor - prevent copying
    NonCopyable(const NonCopyable&) = delete;

    // Delete assignment operator - prevent assignment
    NonCopyable& operator=(const NonCopyable&) = delete;

    // Explicitly default the destructor
    ~NonCopyable() {
        cout << "NonCopyable destroyed" << endl;
        delete data;
    }

    void display() {
        cout << "Data: " << *data << endl;
    }
};

class OnlyMovable {
private:
    int* data;

public:
    OnlyMovable(int val) {
        data = new int(val);
        cout << "OnlyMovable created" << endl;
    }

    // Delete copy constructor and assignment
    OnlyMovable(const OnlyMovable&) = delete;
    OnlyMovable& operator=(const OnlyMovable&) = delete;

    // Move constructor (C++11)
    OnlyMovable(OnlyMovable&& other) noexcept {
        data = other.data;
        other.data = nullptr;
        cout << "OnlyMovable moved" << endl;
    }

    ~OnlyMovable() {
        if (data) {
            delete data;
            cout << "OnlyMovable destroyed" << endl;
        }
    }

    void display() {
        if (data) {
            cout << "Data: " << *data << endl;
        }
    }
};

// ============================================================================
// EXAMPLE 6: CONVERSION CONSTRUCTOR WITH EXPLICIT
// ============================================================================

class Array {
private:
    int* arr;
    int size;

public:
    // Conversion constructor (int -> Array)
    explicit Array(int s) : size(s) {
        arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = 0;
        }
        cout << "Array created with size: " << size << endl;
    }

    // Copy constructor
    Array(const Array& other) : size(other.size) {
        arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = other.arr[i];
        }
        cout << "Array copied" << endl;
    }

    ~Array() {
        delete[] arr;
        cout << "Array destroyed" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            arr[index] = value;
        }
    }

    void display() {
        cout << "Array: [";
        for (int i = 0; i < size; i++) {
            cout << arr[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

void processArray(Array a) {
    a.display();
}

// ============================================================================
// EXAMPLE 7: REAL-WORLD - LOGGER WITH SINGLETON
// ============================================================================

class Logger {
private:
    static Logger* instance;
    string logFile;
    vector<string> logs;

    // Private constructor
    Logger(string file) : logFile(file) {
        cout << "Logger initialized: " << logFile << endl;
    }

public:
    // Delete copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger("app.log");
        }
        return instance;
    }

    void log(string message) {
        logs.push_back(message);
        cout << "[LOG] " << message << endl;
    }

    void showLogs() {
        cout << "\n=== Log History ===" << endl;
        for (const auto& msg : logs) {
            cout << "  - " << msg << endl;
        }
    }

    ~Logger() {
        cout << "Logger closed" << endl;
    }
};

Logger* Logger::instance = nullptr;

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: EXPLICIT KEYWORD ===" << endl;
    
    // Without explicit - implicit conversion works
    Distance d1 = 100;  // OK: implicit conversion from int to Distance
    d1.display();
    printDistance(100);  // OK: 100 converted to Distance object
    
    // With explicit - must use explicit conversion
    // DistanceExplicit d2 = 200;  // ERROR! Implicit conversion not allowed
    DistanceExplicit d2(200);  // OK: explicit construction
    d2.display();
    // printDistanceExplicit(300);  // ERROR! Cannot implicitly convert
    printDistanceExplicit(DistanceExplicit(300));  // OK: explicit conversion
    
    cout << "\n=== EXAMPLE 2: SINGLETON PATTERN ===" << endl;
    
    // Cannot create directly
    // DatabaseConnection db("server");  // ERROR! Constructor is private
    
    // Get singleton instance
    DatabaseConnection* db1 = DatabaseConnection::getInstance();
    db1->executeQuery("SELECT * FROM users");
    
    DatabaseConnection* db2 = DatabaseConnection::getInstance();
    db2->executeQuery("INSERT INTO logs VALUES (...)");
    
    cout << "db1 and db2 are same instance: " << (db1 == db2 ? "Yes" : "No") << endl;
    
    cout << "\n=== EXAMPLE 3: FACTORY PATTERN ===" << endl;
    
    // Cannot create directly
    // Product p("Phone", 50000, "Electronics");  // ERROR! Constructor is private
    
    // Use factory methods
    Product laptop = Product::createElectronics("Laptop", 75000);
    laptop.display();
    
    Product shirt = Product::createClothing("T-Shirt", 999);
    shirt.display();
    
    Product pizza = Product::createFood("Pizza", 399);
    pizza.display();
    
    cout << "\n=== EXAMPLE 4: DELEGATING CONSTRUCTORS ===" << endl;
    
    cout << "\nCreating employee with all parameters:" << endl;
    Employee emp1("Amit Kumar", 30, 60000, "IT");
    emp1.display();
    
    cout << "\nCreating employee with name and age (delegates):" << endl;
    Employee emp2("Priya Singh", 28);
    emp2.display();
    
    cout << "\nCreating employee with name only (double delegation):" << endl;
    Employee emp3("Rahul");
    emp3.display();
    
    cout << "\nCreating default employee (triple delegation):" << endl;
    Employee emp4;
    emp4.display();
    
    cout << "\n=== EXAMPLE 5: DELETE AND DEFAULT ===" << endl;
    
    NonCopyable nc1(42);
    nc1.display();
    
    // Cannot copy
    // NonCopyable nc2 = nc1;  // ERROR! Copy constructor deleted
    // NonCopyable nc3(nc1);   // ERROR! Copy constructor deleted
    
    cout << "\nOnlyMovable example:" << endl;
    OnlyMovable om1(100);
    om1.display();
    
    // Cannot copy
    // OnlyMovable om2 = om1;  // ERROR! Copy constructor deleted
    
    // Can move (C++11)
    OnlyMovable om2 = std::move(om1);
    om2.display();
    
    cout << "\n=== EXAMPLE 6: EXPLICIT WITH ARRAYS ===" << endl;
    
    Array arr1(5);
    arr1.set(0, 10);
    arr1.set(1, 20);
    arr1.display();
    
    // Cannot implicitly convert int to Array
    // processArray(10);  // ERROR! Explicit constructor
    processArray(Array(3));  // OK: explicit conversion
    
    cout << "\n=== EXAMPLE 7: REAL-WORLD LOGGER ===" << endl;
    
    Logger* logger1 = Logger::getInstance();
    logger1->log("Application started");
    logger1->log("User logged in");
    
    Logger* logger2 = Logger::getInstance();
    logger2->log("Data saved successfully");
    
    cout << "\nlogger1 and logger2 are same: " << (logger1 == logger2 ? "Yes" : "No") << endl;
    
    logger1->showLogs();
    
    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. EXPLICIT: Prevents implicit type conversions" << endl;
    cout << "2. PRIVATE CONSTRUCTOR: Singleton, Factory patterns" << endl;
    cout << "3. DELEGATING: Code reuse, reduces duplication (C++11)" << endl;
    cout << "4. = DELETE: Explicitly prevent copying/assignment" << endl;
    cout << "5. = DEFAULT: Explicitly request compiler-generated version" << endl;
    cout << "6. SINGLETON: One instance globally, controlled access" << endl;
    cout << "7. FACTORY: Controlled object creation with validation" << endl;

    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is the explicit keyword and why use it?
 * A1: The 'explicit' keyword prevents implicit type conversions via constructors.
 *     Without explicit, a single-parameter constructor acts as a conversion operator.
 *     Use explicit to avoid unintended conversions that can cause bugs.
 *     Example: explicit Array(int size) prevents Array arr = 10;
 * 
 * Q2: When should you make a constructor private?
 * A2: When you want to control object creation:
 *     - Singleton pattern (only one instance)
 *     - Factory pattern (controlled creation with validation)
 *     - Utility classes (prevent instantiation)
 *     Objects can still be created via static methods or friend functions.
 * 
 * Q3: What are delegating constructors (C++11)?
 * A3: Constructors that call other constructors of the same class.
 *     Syntax: Constructor() : Constructor(params) { }
 *     Benefits: Code reuse, single initialization logic, cleaner code.
 *     The delegating constructor ONLY delegates, cannot initialize members.
 * 
 * Q4: What does = delete mean?
 * A4: Explicitly deletes a function, preventing its use.
 *     Common use: Delete copy constructor/assignment to make class non-copyable.
 *     Example: MyClass(const MyClass&) = delete;
 *     Trying to copy will give compile error, not link error.
 * 
 * Q5: What does = default mean?
 * A5: Explicitly requests compiler-generated default implementation.
 *     Example: MyClass() = default;  // Use compiler's default constructor
 *     Useful when you've defined other constructors but want default one too.
 *     More explicit than just omitting the constructor.
 * 
 * Q6: Explain the Singleton pattern.
 * A6: Design pattern ensuring a class has only ONE instance globally.
 *     Implementation:
 *     - Private constructor (cannot create objects directly)
 *     - Static getInstance() method
 *     - Static instance variable
 *     - Delete copy constructor/assignment
 *     Use cases: Database connections, loggers, configuration managers.
 * 
 * Q7: What is the Factory pattern?
 * A7: Design pattern where object creation is handled by factory methods.
 *     Private constructor prevents direct creation.
 *     Public static factory methods create objects with validation/logic.
 *     Benefits: Controlled creation, validation, different initialization logic.
 * 
 * Q8: Can delegating constructor also initialize members?
 * A8: NO! A delegating constructor can ONLY delegate, nothing else.
 *     Constructor() : BaseClass(), member(value) { }  // OK
 *     Constructor() : Constructor(params), member(value) { }  // ERROR!
 *     Either delegate OR initialize, not both.
 * 
 * Q9: What's the difference between private constructor and = delete?
 * A9: Private constructor: Can still be called by friends/static methods.
 *                         Used for controlled access patterns.
 *     = delete:          Cannot be called by ANYONE, even friends.
 *                         Used to completely prevent usage.
 * 
 * Q10: When should you use explicit keyword?
 * A10: Use explicit for:
 *      - Single-parameter constructors (prevent implicit conversion)
 *      - Multi-parameter constructors if implicit conversion undesired
 *      - Conversion operators
 *      Default rule: Make ALL single-parameter constructors explicit
 *      unless you specifically want implicit conversion.
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. EXPLICIT KEYWORD:
 *    - Prevents implicit conversions
 *    - Only applies to constructors and conversion operators
 *    - Good practice: Use explicit for all single-parameter constructors
 *    - Exception: When you intentionally want implicit conversion
 * 
 * 2. PRIVATE CONSTRUCTOR PATTERNS:
 *    
 *    Singleton:
 *    class Singleton {
 *    private:
 *        static Singleton* instance;
 *        Singleton() { }
 *    public:
 *        static Singleton* getInstance() {
 *            if (!instance) instance = new Singleton();
 *            return instance;
 *        }
 *    };
 *    
 *    Factory:
 *    class Product {
 *    private:
 *        Product(params) { }
 *    public:
 *        static Product create(params) {
 *            // Validation logic
 *            return Product(params);
 *        }
 *    };
 * 
 * 3. DELEGATING CONSTRUCTORS (C++11):
 *    - Must be the ONLY thing in initialization list
 *    - Cannot mix delegation with member initialization
 *    - Can delegate to any constructor of same class
 *    - Reduces code duplication
 *    - Common initialization logic in one place
 * 
 * 4. DELETE VS PRIVATE:
 *    = delete:          Compile error, clearer intent, cannot be called by anyone
 *    private:           Link error, can be called by friends/members
 * 
 * 5. DEFAULT KEYWORD:
 *    - Explicitly request compiler-generated version
 *    - More explicit than omitting
 *    - Useful when you want default implementation but defined other constructors
 *    Example: MyClass() = default;  // Explicit default constructor
 * 
 * 6. SINGLETON CONSIDERATIONS:
 *    Pros:
 *    - Controlled access to single instance
 *    - Lazy initialization
 *    - Global access point
 *    
 *    Cons:
 *    - Global state (harder to test)
 *    - Not thread-safe by default (need mutex)
 *    - Violates Single Responsibility Principle
 *    
 *    Thread-safe singleton (C++11):
 *    static Singleton& getInstance() {
 *        static Singleton instance;  // Thread-safe since C++11
 *        return instance;
 *    }
 * 
 * 7. COMMON PITFALLS:
 *    - Forgetting explicit on single-parameter constructors
 *    - Trying to mix delegation with member initialization
 *    - Not deleting copy constructor in singleton
 *    - Singleton not thread-safe in multi-threaded environment
 *    - Factory methods returning by value (expensive for large objects)
 * 
 * 8. BEST PRACTICES:
 *    - Use explicit for all single-parameter constructors by default
 *    - Delete copy/assignment in singleton and non-copyable classes
 *    - Use delegating constructors to reduce duplication
 *    - Consider thread safety in singleton pattern
 *    - Document why constructor is private
 *    - Prefer modern C++11 features (delete, default, delegating)
 * 
 * 9. C++11 MOVE SEMANTICS:
 *    - Move constructor: Transfer resources instead of copying
 *    - Use when you want move but not copy
 *    - Syntax: MyClass(MyClass&& other) noexcept { }
 *    - Common in unique_ptr, unique resources
 * 
 * 10. WHEN TO USE EACH:
 *     explicit:          Always for single-param constructors
 *     Private:           Singleton, Factory, Utility classes
 *     Delegating:        Multiple constructors with shared logic
 *     = delete:          Non-copyable classes, specific overloads
 *     = default:         Want default but defined other constructors
 */
