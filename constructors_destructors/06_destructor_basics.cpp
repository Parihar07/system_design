#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/*
 * DESTRUCTORS
 * ===========
 * A destructor is a special member function that is called automatically when
 * an object is destroyed (goes out of scope or is explicitly deleted).
 * 
 * INTERVIEW QUESTION: What is a destructor?
 * ANSWER: A destructor is a special member function with the same name as the
 * class prefixed with ~ (tilde). It has no return type and no parameters. It's
 * called automatically when an object is destroyed to clean up resources like
 * dynamically allocated memory, file handles, network connections, etc.
 */

// ============================================================================
// EXAMPLE 1: BASIC DESTRUCTOR
// ============================================================================

class Simple {
private:
    int id;
    string name;

public:
    // Constructor
    Simple(int i, string n) : id(i), name(n) {
        cout << "Constructor: Object " << id << " (" << name << ") created" << endl;
    }

    // Destructor
    ~Simple() {
        cout << "Destructor: Object " << id << " (" << name << ") destroyed" << endl;
    }

    void display() {
        cout << "ID: " << id << ", Name: " << name << endl;
    }
};

// ============================================================================
// EXAMPLE 2: DESTRUCTOR WITH DYNAMIC MEMORY (MUST HAVE!)
// ============================================================================

class DynamicArray {
private:
    int* arr;
    int size;

public:
    // Constructor - Allocates memory
    DynamicArray(int s) : size(s) {
        arr = new int[size];
        cout << "Constructor: Allocated memory for " << size << " integers at " << arr << endl;
        
        // Initialize array
        for(int i = 0; i < size; i++) {
            arr[i] = i * 10;
        }
    }

    // Destructor - MUST deallocate memory
    ~DynamicArray() {
        cout << "Destructor: Deallocating memory at " << arr << endl;
        delete[] arr;  // CRITICAL: Free the allocated memory
        arr = nullptr; // Good practice
    }

    void display() {
        cout << "Array: ";
        for(int i = 0; i < size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

// ============================================================================
// EXAMPLE 3: WHEN IS DESTRUCTOR CALLED?
// ============================================================================

class LifecycleDemo {
private:
    string name;

public:
    LifecycleDemo(string n) : name(n) {
        cout << "  Constructor: " << name << endl;
    }

    ~LifecycleDemo() {
        cout << "  Destructor: " << name << endl;
    }
};

void functionScope() {
    cout << "Entering function" << endl;
    LifecycleDemo obj1("Local Object");
    cout << "Exiting function" << endl;
    // Destructor called here automatically when obj1 goes out of scope
}

// ============================================================================
// EXAMPLE 4: ORDER OF DESTRUCTION
// ============================================================================

class OrderDemo {
private:
    int id;

public:
    OrderDemo(int i) : id(i) {
        cout << "  Constructor: Object " << id << endl;
    }

    ~OrderDemo() {
        cout << "  Destructor: Object " << id << endl;
    }
};

// ============================================================================
// EXAMPLE 5: DESTRUCTOR WITH FILE HANDLING
// ============================================================================

class FileHandler {
private:
    string filename;
    bool isOpen;

public:
    FileHandler(string fname) : filename(fname), isOpen(false) {
        cout << "FileHandler: Opening file '" << filename << "'" << endl;
        // In real code: open actual file
        isOpen = true;
    }

    ~FileHandler() {
        if(isOpen) {
            cout << "FileHandler: Closing file '" << filename << "'" << endl;
            // In real code: close actual file
            isOpen = false;
        }
    }

    void write(string data) {
        if(isOpen) {
            cout << "Writing to " << filename << ": " << data << endl;
        }
    }
};

// ============================================================================
// EXAMPLE 6: MEMORY LEAK WITHOUT DESTRUCTOR
// ============================================================================

class MemoryLeakExample {
private:
    int* data;

public:
    MemoryLeakExample(int size) {
        data = new int[size];
        cout << "Memory allocated at: " << data << endl;
    }

    // NO DESTRUCTOR!
    // Memory leak: allocated memory never freed!
};

class MemoryLeakFixed {
private:
    int* data;
    int size;

public:
    MemoryLeakFixed(int s) : size(s) {
        data = new int[size];
        cout << "Memory allocated at: " << data << endl;
    }

    // Destructor fixes the leak
    ~MemoryLeakFixed() {
        cout << "Memory freed at: " << data << endl;
        delete[] data;
    }
};

// ============================================================================
// EXAMPLE 7: REAL-WORLD - DATABASE CONNECTION
// ============================================================================

class DatabaseConnection {
private:
    string connectionString;
    bool connected;

public:
    DatabaseConnection(string connStr) : connectionString(connStr), connected(false) {
        cout << "DB: Connecting to database..." << endl;
        // Simulate connection
        connected = true;
        cout << "DB: Connected successfully!" << endl;
    }

    ~DatabaseConnection() {
        if(connected) {
            cout << "DB: Disconnecting from database..." << endl;
            // Cleanup: close connection
            connected = false;
            cout << "DB: Disconnected successfully!" << endl;
        }
    }

    void executeQuery(string query) {
        if(connected) {
            cout << "DB: Executing query: " << query << endl;
        }
    }
};

// ============================================================================
// EXAMPLE 8: RULE OF THREE DEMONSTRATION
// ============================================================================

class Student {
private:
    int rollNo;
    char* name;

public:
    // 1. Constructor
    Student(int r, string n) : rollNo(r) {
        name = new char[n.length() + 1];
        strcpy(name, n.c_str());
        cout << "Constructor: " << name << endl;
    }

    // 2. Copy Constructor
    Student(const Student& s) : rollNo(s.rollNo) {
        name = new char[strlen(s.name) + 1];
        strcpy(name, s.name);
        cout << "Copy Constructor: " << name << endl;
    }

    // 3. Destructor (MUST HAVE if using new/delete)
    ~Student() {
        cout << "Destructor: " << name << endl;
        delete[] name;
    }

    void display() {
        cout << "Roll: " << rollNo << ", Name: " << name << endl;
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: BASIC DESTRUCTOR ===" << endl;
    {
        Simple s1(1, "Object-A");
        Simple s2(2, "Object-B");
        s1.display();
        s2.display();
        cout << "End of scope" << endl;
        // Destructors called automatically here (reverse order: s2, then s1)
    }
    cout << "After scope" << endl;

    cout << "\n=== EXAMPLE 2: DYNAMIC MEMORY ===" << endl;
    {
        DynamicArray arr(5);
        arr.display();
        cout << "End of scope" << endl;
        // Destructor frees memory automatically
    }
    cout << "Memory cleaned up!" << endl;

    cout << "\n=== EXAMPLE 3: FUNCTION SCOPE ===" << endl;
    functionScope();
    cout << "Back in main" << endl;

    cout << "\n=== EXAMPLE 4: ORDER OF DESTRUCTION ===" << endl;
    cout << "Creating objects:" << endl;
    {
        OrderDemo obj1(1);
        OrderDemo obj2(2);
        OrderDemo obj3(3);
        cout << "All objects created" << endl;
        cout << "Leaving scope..." << endl;
        // Destroyed in REVERSE order: 3, 2, 1
    }
    cout << "All objects destroyed" << endl;

    cout << "\n=== EXAMPLE 5: FILE HANDLING ===" << endl;
    {
        FileHandler file("data.txt");
        file.write("Hello World");
        file.write("Interview Prep");
        // File automatically closed by destructor
    }

    cout << "\n=== EXAMPLE 6: MEMORY LEAK vs FIX ===" << endl;
    cout << "Without destructor (MEMORY LEAK):" << endl;
    {
        MemoryLeakExample leak(100);
        // Memory never freed! Leak!
    }
    
    cout << "\nWith destructor (FIXED):" << endl;
    {
        MemoryLeakFixed fixed(100);
        // Memory properly freed
    }

    cout << "\n=== EXAMPLE 7: DATABASE CONNECTION ===" << endl;
    {
        DatabaseConnection db("server=localhost;db=testdb");
        db.executeQuery("SELECT * FROM users");
        db.executeQuery("INSERT INTO logs VALUES ('test')");
        // Connection automatically closed by destructor
    }
    cout << "Database operations complete" << endl;

    cout << "\n=== EXAMPLE 8: RULE OF THREE ===" << endl;
    {
        Student s1(101, "Alice");
        Student s2 = s1;  // Copy constructor
        s1.display();
        s2.display();
        // Destructors clean up both objects properly
    }

    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Destructor: ~ClassName() - no parameters, no return type" << endl;
    cout << "2. Called automatically when object is destroyed" << endl;
    cout << "3. MUST have destructor if using dynamic memory (new/delete)" << endl;
    cout << "4. Destroyed in REVERSE order of creation" << endl;
    cout << "5. Only ONE destructor per class (no overloading)" << endl;
    cout << "6. Use for cleanup: memory, files, connections, locks" << endl;
    cout << "7. Rule of Three: Destructor + Copy Constructor + Copy Assignment" << endl;

    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is a destructor?
 * A1: A special member function (~ClassName) called automatically when an object
 *     is destroyed. Used to release resources like memory, files, locks, etc.
 * 
 * Q2: When is a destructor called?
 * A2: - When object goes out of scope
 *     - When delete is called on a pointer to object
 *     - When program terminates (for global/static objects)
 *     - When exception is thrown and stack unwinding occurs
 * 
 * Q3: Can you overload a destructor?
 * A3: No! A class can have only ONE destructor. It takes no parameters and
 *     has no return type.
 * 
 * Q4: Can a destructor be virtual?
 * A4: Yes! And it SHOULD be virtual if the class is meant to be a base class.
 *     This ensures proper cleanup in inheritance hierarchies.
 * 
 * Q5: What is the Rule of Three?
 * A5: If a class needs a custom destructor, it probably needs:
 *     1. Destructor (to clean up resources)
 *     2. Copy Constructor (for deep copying)
 *     3. Copy Assignment Operator (for deep copying)
 * 
 * Q6: What happens if you don't define a destructor?
 * A6: Compiler provides a default destructor that does nothing special (just
 *     calls destructors of member objects). Fine for simple classes, but causes
 *     memory leaks if you have dynamic memory allocation.
 * 
 * Q7: In what order are destructors called?
 * A7: REVERSE order of construction (Last In, First Out - LIFO)
 *     If objects created as: A, B, C
 *     Destructors called as: ~C, ~B, ~A
 * 
 * Q8: Can a destructor throw exceptions?
 * A8: NO! Destructors should NEVER throw exceptions. It can lead to undefined
 *     behavior, especially during stack unwinding. Mark destructors as noexcept.
 * 
 * Q9: Can you explicitly call a destructor?
 * A9: Technically yes: obj.~ClassName()
 *     But you should NEVER do this unless you're doing placement new/delete.
 *     Let the compiler call destructors automatically.
 * 
 * Q10: What is RAII?
 * A10: Resource Acquisition Is Initialization - a design pattern where resource
 *      lifetime is bound to object lifetime. Constructor acquires resource,
 *      destructor releases it. Prevents resource leaks.
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. DESTRUCTOR SYNTAX:
 *    ~ClassName()                                         
 *    - Name: ~ClassName (tilde + class name)
 *    - No return type
 *    - No parameters
 *    - Only ONE per class
 * 
 * 2. WHEN TO DEFINE DESTRUCTOR:
 *    - Class allocates memory with new/malloc
 *    - Class opens files or network connections
 *    - Class acquires locks or other resources
 *    - Class manages any resource that needs cleanup
 * 
 * 3. DESTRUCTOR ORDER:
 *    - Member destructors called after class destructor body
 *    - Base class destructor called after derived class destructor
 *    - Objects destroyed in REVERSE order of creation
 * 
 * 4. VIRTUAL DESTRUCTORS:
 *    - ALWAYS make destructor virtual in base classes
 *    - Prevents resource leaks in inheritance
 *    - Allows proper cleanup of derived objects through base pointers
 * 
 * 5. RULE OF FIVE (Modern C++11):
 *    Rule of Three + Move Constructor + Move Assignment Operator
 *    If you define any of these, consider defining all five.
 * 
 * 6. COMMON MISTAKES:
 *    - Forgetting to delete dynamically allocated memory
 *    - Using delete instead of delete[] for arrays
 *    - Not making destructor virtual in base classes
 *    - Throwing exceptions from destructors
 *    - Accessing member variables in destructor that are already destroyed
 * 
 * 7. BEST PRACTICES:
 *    - Keep destructors simple and fast
 *    - Don't call virtual functions in destructors
 *    - Use RAII pattern for automatic resource management
 *    - Mark destructors as noexcept
 *    - Consider using smart pointers (unique_ptr, shared_ptr) to avoid
 *      manual memory management
 */
