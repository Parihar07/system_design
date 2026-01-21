#include <iostream>
#include <string>
using namespace std;

/*
 * CONSTRUCTOR & DESTRUCTOR ORDER IN INHERITANCE
 * ==============================================
 * Understanding the order in which constructors and destructors are called
 * in inheritance hierarchies is crucial for proper resource management.
 * 
 * INTERVIEW QUESTION: What is the order of constructor and destructor calls in inheritance?
 * ANSWER: Constructors are called from BASE to DERIVED (top-down).
 *         Destructors are called from DERIVED to BASE (bottom-up, reverse order).
 *         This ensures base class resources are available when derived class is constructed,
 *         and derived class resources are cleaned up before base class resources.
 */

// ============================================================================
// EXAMPLE 1: SIMPLE INHERITANCE - ORDER DEMONSTRATION
// ============================================================================

class Base {
public:
    Base() {
        cout << "  1. Base Constructor" << endl;
    }

    ~Base() {
        cout << "  4. Base Destructor" << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "  2. Derived Constructor" << endl;
    }

    ~Derived() {
        cout << "  3. Derived Destructor" << endl;
    }
};

// ============================================================================
// EXAMPLE 2: MULTI-LEVEL INHERITANCE
// ============================================================================

class Grandparent {
public:
    Grandparent() {
        cout << "  1. Grandparent Constructor" << endl;
    }

    ~Grandparent() {
        cout << "  6. Grandparent Destructor" << endl;
    }
};

class Parent : public Grandparent {
public:
    Parent() {
        cout << "  2. Parent Constructor" << endl;
    }

    ~Parent() {
        cout << "  5. Parent Destructor" << endl;
    }
};

class Child : public Parent {
public:
    Child() {
        cout << "  3. Child Constructor" << endl;
    }

    ~Child() {
        cout << "  4. Child Destructor" << endl;
    }
};

// ============================================================================
// EXAMPLE 3: WITH MEMBER OBJECTS
// ============================================================================

class Engine {
public:
    Engine() {
        cout << "    → Engine Constructor" << endl;
    }

    ~Engine() {
        cout << "    → Engine Destructor" << endl;
    }
};

class Wheels {
public:
    Wheels() {
        cout << "    → Wheels Constructor" << endl;
    }

    ~Wheels() {
        cout << "    → Wheels Destructor" << endl;
    }
};

class Vehicle {
private:
    Engine engine;  // Member object
public:
    Vehicle() {
        cout << "  1. Vehicle Constructor" << endl;
    }

    ~Vehicle() {
        cout << "  4. Vehicle Destructor" << endl;
    }
};

class Car : public Vehicle {
private:
    Wheels wheels;  // Member object
public:
    Car() {
        cout << "  3. Car Constructor" << endl;
    }

    ~Car() {
        cout << "  2. Car Destructor" << endl;
    }
};

// ============================================================================
// EXAMPLE 4: PARAMETERIZED CONSTRUCTORS WITH INHERITANCE
// ============================================================================

class Person {
protected:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {
        cout << "  Person Constructor: " << name << ", Age: " << age << endl;
    }

    ~Person() {
        cout << "  Person Destructor: " << name << endl;
    }
};

class Employee : public Person {
private:
    int empId;
    double salary;

public:
    // MUST call base class constructor explicitly
    Employee(string n, int a, int id, double sal) 
        : Person(n, a), empId(id), salary(sal) {
        cout << "  Employee Constructor: ID " << empId << ", Salary: ₹" << salary << endl;
    }

    ~Employee() {
        cout << "  Employee Destructor: ID " << empId << endl;
    }

    void display() {
        cout << "Name: " << name << ", Age: " << age;
        cout << ", ID: " << empId << ", Salary: ₹" << salary << endl;
    }
};

// ============================================================================
// EXAMPLE 5: MULTIPLE INHERITANCE (ADVANCED)
// ============================================================================

class ClassA {
public:
    ClassA() {
        cout << "  1. ClassA Constructor" << endl;
    }

    ~ClassA() {
        cout << "  6. ClassA Destructor" << endl;
    }
};

class ClassB {
public:
    ClassB() {
        cout << "  2. ClassB Constructor" << endl;
    }

    ~ClassB() {
        cout << "  5. ClassB Destructor" << endl;
    }
};

class ClassC : public ClassA, public ClassB {
public:
    ClassC() {
        cout << "  3. ClassC Constructor" << endl;
    }

    ~ClassC() {
        cout << "  4. ClassC Destructor" << endl;
    }
};

// ============================================================================
// EXAMPLE 6: VIRTUAL DESTRUCTOR (IMPORTANT!)
// ============================================================================

class Shape {
public:
    Shape() {
        cout << "  Shape Constructor" << endl;
    }

    // Non-virtual destructor (DANGEROUS for inheritance!)
    ~Shape() {
        cout << "  Shape Destructor" << endl;
    }

    virtual void draw() {
        cout << "Drawing Shape" << endl;
    }
};

class Circle : public Shape {
private:
    int* radius;

public:
    Circle(int r) {
        radius = new int(r);
        cout << "  Circle Constructor (allocated memory)" << endl;
    }

    ~Circle() {
        cout << "  Circle Destructor (freeing memory)" << endl;
        delete radius;
    }

    void draw() override {
        cout << "Drawing Circle with radius " << *radius << endl;
    }
};

// Fixed version with virtual destructor
class ShapeFixed {
public:
    ShapeFixed() {
        cout << "  ShapeFixed Constructor" << endl;
    }

    // Virtual destructor (CORRECT!)
    virtual ~ShapeFixed() {
        cout << "  ShapeFixed Destructor" << endl;
    }

    virtual void draw() {
        cout << "Drawing ShapeFixed" << endl;
    }
};

class CircleFixed : public ShapeFixed {
private:
    int* radius;

public:
    CircleFixed(int r) {
        radius = new int(r);
        cout << "  CircleFixed Constructor (allocated memory)" << endl;
    }

    ~CircleFixed() {
        cout << "  CircleFixed Destructor (freeing memory)" << endl;
        delete radius;
    }

    void draw() override {
        cout << "Drawing CircleFixed with radius " << *radius << endl;
    }
};

// ============================================================================
// EXAMPLE 7: REAL-WORLD - ACCOUNT HIERARCHY
// ============================================================================

class Account {
protected:
    string accountNumber;
    double balance;

public:
    Account(string accNum, double bal) : accountNumber(accNum), balance(bal) {
        cout << "  Account Constructor: " << accountNumber << endl;
    }

    virtual ~Account() {
        cout << "  Account Destructor: " << accountNumber << endl;
    }

    virtual void display() {
        cout << "Account: " << accountNumber << ", Balance: ₹" << balance << endl;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(string accNum, double bal, double rate)
        : Account(accNum, bal), interestRate(rate) {
        cout << "  SavingsAccount Constructor: Rate " << interestRate << "%" << endl;
    }

    ~SavingsAccount() {
        cout << "  SavingsAccount Destructor" << endl;
    }

    void display() override {
        cout << "Savings Account: " << accountNumber;
        cout << ", Balance: ₹" << balance;
        cout << ", Interest: " << interestRate << "%" << endl;
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: SIMPLE INHERITANCE ===" << endl;
    cout << "Creating Derived object:" << endl;
    {
        Derived d;
        cout << "Object in use..." << endl;
    }
    cout << "Object destroyed\n" << endl;

    cout << "=== EXAMPLE 2: MULTI-LEVEL INHERITANCE ===" << endl;
    cout << "Creating Child object:" << endl;
    {
        Child c;
        cout << "Object in use..." << endl;
    }
    cout << "Object destroyed\n" << endl;

    cout << "=== EXAMPLE 3: WITH MEMBER OBJECTS ===" << endl;
    cout << "Creating Car object:" << endl;
    {
        Car car;
        cout << "Object in use..." << endl;
    }
    cout << "Object destroyed\n" << endl;

    cout << "Order Summary:" << endl;
    cout << "Construction: Base members → Base class → Derived members → Derived class" << endl;
    cout << "Destruction:  Derived class → Derived members → Base class → Base members\n" << endl;

    cout << "=== EXAMPLE 4: PARAMETERIZED CONSTRUCTORS ===" << endl;
    {
        Employee emp("Rajesh Kumar", 30, 1001, 50000);
        emp.display();
    }
    cout << endl;

    cout << "=== EXAMPLE 5: MULTIPLE INHERITANCE ===" << endl;
    cout << "Creating ClassC object (inherits from A and B):" << endl;
    {
        ClassC obj;
    }
    cout << "Order: A → B → C (construction), C → B → A (destruction)\n" << endl;

    cout << "=== EXAMPLE 6: VIRTUAL DESTRUCTOR IMPORTANCE ===" << endl;
    
    cout << "\nWithout virtual destructor (MEMORY LEAK!):" << endl;
    {
        Shape* s = new Circle(5);
        s->draw();
        delete s;  // Only ~Shape() called, ~Circle() NOT called! Memory leak!
    }

    cout << "\nWith virtual destructor (CORRECT):" << endl;
    {
        ShapeFixed* s = new CircleFixed(5);
        s->draw();
        delete s;  // Both ~CircleFixed() and ~ShapeFixed() called! No leak!
    }
    cout << endl;

    cout << "=== EXAMPLE 7: REAL-WORLD ACCOUNT ===" << endl;
    {
        Account* acc = new SavingsAccount("SA12345", 10000, 4.5);
        acc->display();
        delete acc;  // Virtual destructor ensures proper cleanup
    }
    cout << endl;

    cout << "=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Constructor Order: BASE → DERIVED (top-down)" << endl;
    cout << "2. Destructor Order: DERIVED → BASE (bottom-up, reverse)" << endl;
    cout << "3. Member objects constructed BEFORE class constructor body" << endl;
    cout << "4. Member objects destroyed AFTER class destructor body" << endl;
    cout << "5. ALWAYS use virtual destructor in base classes!" << endl;
    cout << "6. Call base constructor explicitly in derived class init list" << endl;
    cout << "7. Multiple inheritance: order determined by declaration order" << endl;

    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is the order of constructor calls in inheritance?
 * A1: BASE class constructor → DERIVED class constructor (top-down).
 *     This ensures base class is fully initialized before derived class uses it.
 * 
 * Q2: What is the order of destructor calls in inheritance?
 * A2: DERIVED class destructor → BASE class destructor (bottom-up, reverse).
 *     This ensures derived class resources are cleaned up before base class resources.
 * 
 * Q3: Why is constructor order base-to-derived?
 * A3: Because the derived class may depend on base class members. The base class
 *     must be fully constructed and ready before the derived class can safely use it.
 * 
 * Q4: Why is destructor order derived-to-base?
 * A4: Because the derived class may still be using base class resources. We must
 *     clean up derived class first, then base class resources.
 * 
 * Q5: When are member objects constructed?
 * A5: Member objects are constructed BEFORE the class constructor body executes,
 *     in the order they are declared in the class (not initialization list order!).
 * 
 * Q6: When are member objects destroyed?
 * A6: Member objects are destroyed AFTER the class destructor body executes,
 *     in REVERSE order of their construction.
 * 
 * Q7: What happens if base class has no default constructor?
 * A7: You MUST explicitly call the base class parameterized constructor in the
 *     derived class initialization list. Otherwise, compilation error.
 * 
 * Q8: Why should base class destructor be virtual?
 * A8: When deleting a derived object through a base class pointer, without virtual
 *     destructor, only base destructor is called → memory leak in derived class!
 *     With virtual destructor, proper destructor chain is called.
 * 
 * Q9: What is the order in multiple inheritance?
 * A9: Constructors called in the order base classes are listed in class declaration.
 *     Destructors called in reverse order.
 * 
 * Q10: Can you call virtual functions in constructor/destructor?
 * A10: Technically yes, but DON'T! During construction/destruction, the object is
 *      not fully formed. Virtual function calls resolve to current class, not
 *      derived class (no polymorphism).
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. CONSTRUCTION ORDER (Detailed):
 *    a. Base class member objects
 *    b. Base class constructor body
 *    c. Derived class member objects
 *    d. Derived class constructor body
 * 
 * 2. DESTRUCTION ORDER (Detailed):
 *    a. Derived class destructor body
 *    b. Derived class member objects (reverse order)
 *    c. Base class destructor body
 *    d. Base class member objects (reverse order)
 * 
 * 3. VIRTUAL DESTRUCTOR RULE:
 *    - Always make destructor virtual if class has virtual functions
 *    - Always make destructor virtual if class is meant to be base class
 *    - Cost: One extra vtable pointer per object (usually worth it!)
 * 
 * 4. CALLING BASE CONSTRUCTOR:
 *    Derived(params) : Base(params) {  // Correct
 *        // Derived constructor body
 *    }
 *    
 *    Cannot do:
 *    Derived(params) {
 *        Base(params);  // ERROR! This creates temporary object
 *    }
 * 
 * 5. MEMBER INITIALIZATION ORDER:
 *    Members initialized in DECLARATION order, not init list order!
 *    class Example {
 *        int a;
 *        int b;
 *    public:
 *        Example() : b(5), a(b) { }  // DANGER! a initialized before b!
 *    };
 * 
 * 6. MULTIPLE INHERITANCE ORDER:
 *    class D : public A, public B, public C { };
 *    Construction: A → B → C → D
 *    Destruction:  D → C → B → A
 * 
 * 7. COMMON PITFALLS:
 *    - Forgetting to call base constructor explicitly
 *    - Not making base destructor virtual
 *    - Calling virtual functions in constructor/destructor
 *    - Depending on member initialization list order instead of declaration order
 *    - Accessing base class members before they're initialized
 * 
 * 8. BEST PRACTICES:
 *    - Always use virtual destructors in base classes
 *    - Explicitly call base constructor in initialization list
 *    - Keep constructors/destructors simple
 *    - Don't call virtual functions in constructor/destructor
 *    - Initialize members in declaration order
 *    - Document initialization dependencies
 */
