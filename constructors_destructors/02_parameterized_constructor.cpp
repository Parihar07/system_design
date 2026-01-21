#include <iostream>
#include <string>
using namespace std;

/*
 * PARAMETERIZED CONSTRUCTORS
 * ==========================
 * A constructor that accepts parameters to initialize object with specific values.
 * 
 * INTERVIEW QUESTION: What is a parameterized constructor?
 * ANSWER: A parameterized constructor is a constructor that accepts arguments
 * to initialize the object with user-defined values at the time of creation.
 * It provides flexibility to create objects with different initial states.
 */

class Student {
private:
    string name;
    int age;
    int rollNumber;

public:
    // Parameterized Constructor with 3 parameters
    Student(string n, int a, int r) {
        cout << "Parameterized Constructor Called!" << endl;
        name = n;
        age = a;
        rollNumber = r;
    }

    void display() {
        cout << "Name: " << name << ", Age: " << age << ", Roll: " << rollNumber << endl;
    }
};

// Example 2: Constructor with default parameters
class Car {
private:
    string brand;
    string model;
    int year;

public:
    // Constructor with default parameters
    Car(string b = "Unknown", string m = "Unknown", int y = 2024) {
        cout << "Car Constructor Called!" << endl;
        brand = b;
        model = m;
        year = y;
    }

    void display() {
        cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << endl;
    }
};

// Example 3: Multiple Constructors (Constructor Overloading Preview)
class Rectangle {
private:
    int length;
    int width;

public:
    // Constructor 1: No parameters (Default)
    Rectangle() {
        cout << "Default Constructor" << endl;
        length = 0;
        width = 0;
    }

    // Constructor 2: One parameter (Square)
    Rectangle(int side) {
        cout << "Square Constructor" << endl;
        length = side;
        width = side;
    }

    // Constructor 3: Two parameters (Rectangle)
    Rectangle(int l, int w) {
        cout << "Rectangle Constructor" << endl;
        length = l;
        width = w;
    }

    void display() {
        cout << "Length: " << length << ", Width: " << width << endl;
    }

    int area() {
        return length * width;
    }
};

// Example 4: Controlled Object Creation (Validation)
class BankAccount {
private:
    string accountHolder;
    double balance;
    int accountNumber;

public:
    // Constructor with validation
    BankAccount(string holder, double initialBalance, int accNum) {
        cout << "Creating Bank Account..." << endl;
        
        accountHolder = holder;
        accountNumber = accNum;
        
        // VALIDATION - Control object creation
        if (initialBalance < 0) {
            cout << "Invalid initial balance! Setting to 0." << endl;
            balance = 0;
        } else if (initialBalance < 500) {
            cout << "Minimum balance required is 500. Setting to 500." << endl;
            balance = 500;
        } else {
            balance = initialBalance;
        }
    }

    void display() {
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: ₹" << balance << endl;
    }
};

int main() {
    cout << "=== PARAMETERIZED CONSTRUCTOR DEMO ===" << endl;
    cout << endl;
    
    // Example 1: Basic Parameterized Constructor
    cout << "1. Creating Student with parameters:" << endl;
    Student s1("Alice", 20, 101);
    s1.display();
    
    cout << "\n2. Creating another Student:" << endl;
    Student s2("Bob", 22, 102);
    s2.display();
    
    // Example 2: Constructor with Default Parameters
    cout << "\n=== DEFAULT PARAMETERS ===" << endl;
    Car c1("Toyota", "Camry", 2023);
    c1.display();
    
    Car c2("Honda", "Civic");  // Uses default year
    c2.display();
    
    Car c3;  // Uses all defaults
    c3.display();
    
    // Example 3: Multiple Constructors
    cout << "\n=== MULTIPLE CONSTRUCTORS ===" << endl;
    Rectangle r1;              // Default constructor
    r1.display();
    
    Rectangle r2(5);           // Square constructor
    r2.display();
    cout << "Area: " << r2.area() << endl;
    
    Rectangle r3(10, 20);      // Rectangle constructor
    r3.display();
    cout << "Area: " << r3.area() << endl;
    
    // Example 4: Controlled Object Creation
    cout << "\n=== CONTROLLED OBJECT CREATION ===" << endl;
    BankAccount acc1("John Doe", 1000, 12345);
    acc1.display();
    
    cout << "\nTrying to create account with negative balance:" << endl;
    BankAccount acc2("Jane Smith", -500, 12346);
    acc2.display();
    
    cout << "\nTrying to create account with low balance:" << endl;
    BankAccount acc3("Mike Johnson", 200, 12347);
    acc3.display();
    
    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Parameterized constructors accept arguments" << endl;
    cout << "2. Allow creating objects with different initial values" << endl;
    cout << "3. Can have default parameter values" << endl;
    cout << "4. Can validate input and enforce business rules" << endl;
    cout << "5. Multiple constructors can coexist (overloading)" << endl;
    
    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is the difference between default and parameterized constructor?
 * A1: Default constructor takes no arguments and initializes objects with default values.
 *     Parameterized constructor takes arguments to initialize objects with specific values.
 * 
 * Q2: Can you have both default and parameterized constructors in a class?
 * A2: Yes, this is called constructor overloading. You can have multiple constructors
 *     with different parameter lists.
 * 
 * Q3: What happens if you define only a parameterized constructor?
 * A3: The compiler will NOT provide a default constructor. You must explicitly create
 *     objects with parameters, or define a default constructor yourself.
 * 
 * Q4: What are default parameters in constructors?
 * A4: Default parameters allow you to specify default values for constructor parameters.
 *     Users can omit these arguments when creating objects.
 * 
 * Q5: How can constructors help in controlled object creation?
 * A5: Constructors can validate input parameters, enforce business rules, ensure
 *     objects are always in a valid state, and prevent creation of invalid objects.
 * 
 * Q6: What is the advantage of parameterized constructors?
 * A6: They allow flexible object creation, eliminate the need for separate setter
 *     methods for initialization, and ensure objects are properly initialized at
 *     creation time.
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. IMPLICIT CONVERSION:
 *    If constructor has ONE parameter, it can act as a type converter
 *    Student s = "John";  // May work if there's Student(string) constructor
 *    Use 'explicit' keyword to prevent this: explicit Student(string n)
 * 
 * 2. NO DEFAULT CONSTRUCTOR:
 *    Once you define ANY constructor, compiler doesn't provide default one
 *    
 * 3. ORDER OF PARAMETERS:
 *    Make commonly used parameters first, optional ones with defaults last
 */
