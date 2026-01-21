#include <iostream>
#include <string>
using namespace std;

/*
 * CONSTRUCTOR INITIALIZATION LISTS
 * =================================
 * A way to initialize member variables directly before the constructor body executes.
 * Syntax: Constructor() : member1(value1), member2(value2) { }
 * 
 * INTERVIEW QUESTION: What is a constructor initialization list?
 * ANSWER: An initialization list is a comma-separated list of member initializations
 * that appears after the constructor parameter list, separated by a colon. It
 * initializes members before the constructor body executes, which is more efficient
 * than assignment inside the constructor body.
 */

// ============================================================================
// EXAMPLE 1: INITIALIZATION LIST vs ASSIGNMENT
// ============================================================================

class Student {
private:
    int rollNo;
    string name;
    float marks;

public:
    // WITHOUT Initialization List (Assignment in body)
    Student(int r, string n, float m) {
        cout << "Constructor using assignment" << endl;
        rollNo = r;     // Assignment (not initialization)
        name = n;       // Assignment (not initialization)
        marks = m;      // Assignment (not initialization)
    }
};

class StudentOptimized {
private:
    int rollNo;
    string name;
    float marks;

public:
    // WITH Initialization List (Better!)
    StudentOptimized(int r, string n, float m) : rollNo(r), name(n), marks(m) {
        cout << "Constructor using initialization list" << endl;
        // Constructor body can be empty or contain additional logic
    }

    void display() {
        cout << "Roll: " << rollNo << ", Name: " << name << ", Marks: " << marks << endl;
    }
};

// ============================================================================
// EXAMPLE 2: WHEN INITIALIZATION LIST IS MANDATORY
// ============================================================================

// Case 1: Const members
class ConstExample {
private:
    const int id;           // const member - MUST use initialization list
    const string category;

public:
    // ❌ This won't compile without initialization list
    // ConstExample(int i, string c) {
    //     id = i;         // ERROR! Can't assign to const
    //     category = c;   // ERROR! Can't assign to const
    // }

    // ✅ Correct way
    ConstExample(int i, string c) : id(i), category(c) {
        cout << "Const members initialized" << endl;
    }

    void display() {
        cout << "ID: " << id << ", Category: " << category << endl;
    }
};

// Case 2: Reference members
class ReferenceExample {
private:
    int& ref;  // Reference member - MUST use initialization list

public:
    // ❌ This won't compile
    // ReferenceExample(int& r) {
    //     ref = r;  // ERROR! References must be initialized
    // }

    // ✅ Correct way
    ReferenceExample(int& r) : ref(r) {
        cout << "Reference initialized" << endl;
    }

    void display() {
        cout << "Reference value: " << ref << endl;
    }
};

// Case 3: Member objects without default constructor
class Engine {
private:
    int horsepower;

public:
    // NO DEFAULT CONSTRUCTOR!
    Engine(int hp) : horsepower(hp) {
        cout << "Engine created with " << hp << " HP" << endl;
    }

    void display() {
        cout << "Engine: " << horsepower << " HP" << endl;
    }
};

class Car {
private:
    Engine engine;  // Member object without default constructor
    string model;

public:
    // ❌ This won't compile
    // Car(string m, int hp) {
    //     engine = Engine(hp);  // ERROR! engine already constructed
    //     model = m;
    // }

    // ✅ Correct way - MUST use initialization list
    Car(string m, int hp) : engine(hp), model(m) {
        cout << "Car created: " << model << endl;
    }

    void display() {
        cout << "Model: " << model << ", ";
        engine.display();
    }
};

// Case 4: Base class without default constructor
class Vehicle {
protected:
    string type;

public:
    // NO DEFAULT CONSTRUCTOR!
    Vehicle(string t) : type(t) {
        cout << "Vehicle created: " << type << endl;
    }
};

class Bike : public Vehicle {
private:
    string brand;

public:
    // MUST call base class constructor in initialization list
    Bike(string t, string b) : Vehicle(t), brand(b) {
        cout << "Bike created: " << brand << endl;
    }

    void display() {
        cout << "Type: " << type << ", Brand: " << brand << endl;
    }
};

// ============================================================================
// EXAMPLE 3: ORDER OF INITIALIZATION
// ============================================================================

class OrderDemo {
private:
    int a;
    int b;
    int c;

public:
    // IMPORTANT: Members are initialized in DECLARATION order, NOT list order!
    OrderDemo(int x) : c(x), b(c + 1), a(b + 1) {
        cout << "Initialization list order: c, b, a" << endl;
        cout << "But actual initialization: a, b, c (declaration order)" << endl;
    }

    void display() {
        cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
        cout << "Notice: Values may be unexpected due to initialization order!" << endl;
    }
};

class OrderFixed {
private:
    int a;
    int b;
    int c;

public:
    // ✅ Correct: List order matches declaration order
    OrderFixed(int x) : a(x), b(a + 1), c(b + 1) {
        cout << "Both orders match" << endl;
    }

    void display() {
        cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
    }
};

// ============================================================================
// EXAMPLE 4: PERFORMANCE COMPARISON
// ============================================================================

class PerformanceTest {
private:
    string str1;
    string str2;
    string str3;

public:
    // Method 1: Assignment (Less efficient)
    // Creates default strings first, then assigns new values
    // Total: 3 default constructions + 3 assignments + 3 destructions
    PerformanceTest(string s1, string s2, string s3) {
        str1 = s1;  // Default construct + assign
        str2 = s2;  // Default construct + assign
        str3 = s3;  // Default construct + assign
    }
};

class PerformanceTestOptimized {
private:
    string str1;
    string str2;
    string str3;

public:
    // Method 2: Initialization List (More efficient)
    // Directly constructs with given values
    // Total: 3 constructions only
    PerformanceTestOptimized(string s1, string s2, string s3) 
        : str1(s1), str2(s2), str3(s3) {
        // Members already initialized!
    }
};

// ============================================================================
// EXAMPLE 5: REAL-WORLD - EMPLOYEE WITH INITIALIZATION LIST
// ============================================================================

class Employee {
private:
    const int empId;           // const - MUST use init list
    string name;
    const string company;      // const - MUST use init list
    double salary;
    int& yearOfJoining;        // reference - MUST use init list

public:
    Employee(int id, string n, string comp, double sal, int& year)
        : empId(id), name(n), company(comp), salary(sal), yearOfJoining(year) {
        cout << "Employee created with initialization list" << endl;
    }

    void display() {
        cout << "ID: " << empId << ", Name: " << name;
        cout << ", Company: " << company << ", Salary: ₹" << salary;
        cout << ", Joined: " << yearOfJoining << endl;
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: INITIALIZATION LIST vs ASSIGNMENT ===" << endl;
    StudentOptimized s1(101, "Rahul", 85.5);
    s1.display();

    cout << "\n=== EXAMPLE 2: MANDATORY CASES ===" << endl;
    
    // Case 1: Const members
    ConstExample c1(1001, "Premium");
    c1.display();

    // Case 2: Reference members
    int year = 2024;
    ReferenceExample r1(year);
    r1.display();

    // Case 3: Member objects without default constructor
    Car car1("Tesla Model 3", 450);
    car1.display();

    // Case 4: Base class without default constructor
    cout << "\nCreating Bike:" << endl;
    Bike bike1("Sports", "Ducati");
    bike1.display();

    cout << "\n=== EXAMPLE 3: INITIALIZATION ORDER ===" << endl;
    cout << "Wrong order in list:" << endl;
    OrderDemo o1(10);
    o1.display();

    cout << "\nCorrect order in list:" << endl;
    OrderFixed o2(10);
    o2.display();

    cout << "\n=== EXAMPLE 5: REAL-WORLD EMPLOYEE ===" << endl;
    int joiningYear = 2020;
    Employee emp1(501, "Priya Sharma", "TechCorp", 75000, joiningYear);
    emp1.display();

    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Initialization list is more efficient than assignment" << endl;
    cout << "2. MANDATORY for: const members, references, base classes" << endl;
    cout << "3. Members initialized in DECLARATION order, not list order" << endl;
    cout << "4. Syntax: Constructor() : member1(val1), member2(val2) { }" << endl;
    cout << "5. Prefer initialization lists over assignment in body" << endl;

    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is a constructor initialization list?
 * A1: A comma-separated list after the constructor signature (after :) that
 *     initializes member variables before the constructor body executes.
 * 
 * Q2: When MUST you use an initialization list?
 * A2: - Const member variables
 *     - Reference member variables
 *     - Member objects that don't have default constructors
 *     - Calling base class constructors (inheritance)
 * 
 * Q3: What's the difference between initialization and assignment?
 * A3: Initialization: Direct construction with value (happens once)
 *     Assignment: Default construction + assignment (two operations)
 *     Initialization is more efficient!
 * 
 * Q4: In what order are members initialized?
 * A4: Members are ALWAYS initialized in the order they are DECLARED in the class,
 *     NOT the order in the initialization list! This can cause bugs if you're
 *     not careful.
 * 
 * Q5: Can you use initialization list for some members and assignment for others?
 * A5: Yes, you can mix both. But for consistency and efficiency, prefer using
 *     initialization list for all members.
 * 
 * Q6: Why is initialization list more efficient?
 * A6: With assignment: Default construct → assign → destroy temporary
 *     With init list: Direct construction with value (one step)
 *     Especially important for complex objects (strings, vectors, etc.)
 * 
 * Q7: Can you initialize static members in initialization list?
 * A7: No! Static members belong to the class, not the object. They must be
 *     initialized outside the class definition.
 * 
 * Q8: What happens if a member is not in the initialization list?
 * A8: It gets default-initialized (default constructor called) before the
 *     constructor body executes. Then you can assign to it in the body.
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. SYNTAX:
 *    Constructor(params) : member1(value1), member2(value2) {
 *        // Constructor body
 *    }
 * 
 * 2. INITIALIZATION ORDER:
 *    Always follows DECLARATION order in class, NOT list order!
 *    This can cause subtle bugs if you depend on one member to initialize another.
 * 
 * 3. BEST PRACTICES:
 *    - Always use initialization list for const and reference members
 *    - Prefer initialization list over assignment for efficiency
 *    - Keep initialization list order same as declaration order
 *    - Use initialization list for all members (consistency)
 * 
 * 4. COMMON MISTAKES:
 *    - Forgetting that order is based on declaration, not list
 *    - Trying to initialize static members in initialization list
 *    - Not using initialization list for const/reference members
 * 
 * 5. MODERN C++ (C++11 onwards):
 *    - Can use brace initialization: member{value}
 *    - Can have default member initializers in class declaration
 *    Example: int count = 0;  // in class declaration
 */
