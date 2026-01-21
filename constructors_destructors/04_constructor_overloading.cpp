#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/*
 * CONSTRUCTOR OVERLOADING
 * =======================
 * Having multiple constructors with different parameter lists in the same class.
 * This is an example of compile-time polymorphism (function overloading).
 * 
 * INTERVIEW QUESTION: What is constructor overloading?
 * ANSWER: Constructor overloading is having multiple constructors in a class
 * with different numbers or types of parameters. The compiler selects the
 * appropriate constructor based on the arguments provided during object creation.
 */

// ============================================================================
// EXAMPLE 1: BASIC CONSTRUCTOR OVERLOADING
// ============================================================================

class BankAccount {
private:
    string accountHolder;
    long accountNumber;
    double balance;
    string accountType;

public:
    // Constructor 1: No parameters (Default)
    BankAccount() {
        cout << "Constructor 1: Default constructor called" << endl;
        accountHolder = "Unknown";
        accountNumber = 0;
        balance = 0.0;
        accountType = "Savings";
    }

    // Constructor 2: Only account holder
    BankAccount(string holder) {
        cout << "Constructor 2: With holder name" << endl;
        accountHolder = holder;
        accountNumber = 100000 + rand() % 900000;  // Generate random
        balance = 0.0;
        accountType = "Savings";
    }

    // Constructor 3: Holder and initial balance
    BankAccount(string holder, double initialBalance) {
        cout << "Constructor 3: With holder and balance" << endl;
        accountHolder = holder;
        accountNumber = 100000 + rand() % 900000;
        balance = initialBalance >= 500 ? initialBalance : 500;  // Min balance
        accountType = "Savings";
    }

    // Constructor 4: Full details
    BankAccount(string holder, double initialBalance, string type) {
        cout << "Constructor 4: With all details" << endl;
        accountHolder = holder;
        accountNumber = 100000 + rand() % 900000;
        balance = initialBalance >= 500 ? initialBalance : 500;
        accountType = type;
    }

    // Constructor 5: With specific account number
    BankAccount(string holder, long accNum, double initialBalance, string type) {
        cout << "Constructor 5: With specific account number" << endl;
        accountHolder = holder;
        accountNumber = accNum;
        balance = initialBalance >= 500 ? initialBalance : 500;
        accountType = type;
    }

    void display() {
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: ₹" << balance << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "-----------------------------------" << endl;
    }
};

// ============================================================================
// EXAMPLE 2: OVERLOADING WITH DIFFERENT TYPES
// ============================================================================

class Rectangle {
private:
    double length;
    double width;

public:
    // Constructor 1: No parameters (default unit square)
    Rectangle() {
        cout << "Rectangle: Default constructor" << endl;
        length = 1.0;
        width = 1.0;
    }

    // Constructor 2: One parameter (square)
    Rectangle(double side) {
        cout << "Rectangle: Square constructor (int)" << endl;
        length = side;
        width = side;
    }

    // Constructor 3: Two parameters (rectangle)
    Rectangle(double l, double w) {
        cout << "Rectangle: Rectangle constructor" << endl;
        length = l;
        width = w;
    }

    // Constructor 4: From another shape (conversion)
    Rectangle(int side) {  // Different type!
        cout << "Rectangle: Integer square constructor" << endl;
        length = (double)side;
        width = (double)side;
    }

    void display() {
        cout << "Length: " << length << ", Width: " << width;
        cout << ", Area: " << length * width << endl;
    }
};

// ============================================================================
// EXAMPLE 3: REAL-WORLD - EMPLOYEE SYSTEM
// ============================================================================

class Employee {
private:
    int empId;
    string name;
    string department;
    double salary;
    string email;

public:
    // Constructor 1: Intern/New Employee (minimal info)
    Employee(int id, string empName) {
        cout << "Employee: Intern constructor" << endl;
        empId = id;
        name = empName;
        department = "Unassigned";
        salary = 0.0;
        email = "";
    }

    // Constructor 2: Regular Employee
    Employee(int id, string empName, string dept) {
        cout << "Employee: Regular constructor" << endl;
        empId = id;
        name = empName;
        department = dept;
        salary = 30000.0;  // Default salary
        email = empName + "@company.com";
    }

    // Constructor 3: Employee with salary
    Employee(int id, string empName, string dept, double sal) {
        cout << "Employee: With salary constructor" << endl;
        empId = id;
        name = empName;
        department = dept;
        salary = sal;
        email = empName + "@company.com";
    }

    // Constructor 4: Complete details
    Employee(int id, string empName, string dept, double sal, string mail) {
        cout << "Employee: Complete details constructor" << endl;
        empId = id;
        name = empName;
        department = dept;
        salary = sal;
        email = mail;
    }

    void display() {
        cout << "ID: " << empId << ", Name: " << name;
        cout << ", Dept: " << department << ", Salary: ₹" << salary;
        if(!email.empty()) cout << ", Email: " << email;
        cout << endl;
    }
};

// ============================================================================
// EXAMPLE 4: OVERLOADING WITH POINTERS/ARRAYS
// ============================================================================

class StudentGrades {
private:
    string name;
    int* grades;
    int numSubjects;

public:
    // Constructor 1: Name only
    StudentGrades(string n) {
        cout << "StudentGrades: Name only" << endl;
        name = n;
        numSubjects = 0;
        grades = nullptr;
    }

    // Constructor 2: Name and number of subjects
    StudentGrades(string n, int subjects) {
        cout << "StudentGrades: Name and subject count" << endl;
        name = n;
        numSubjects = subjects;
        grades = new int[numSubjects];
        // Initialize all grades to 0
        for(int i = 0; i < numSubjects; i++) {
            grades[i] = 0;
        }
    }

    // Constructor 3: Name and grades array
    StudentGrades(string n, int* gradeArray, int subjects) {
        cout << "StudentGrades: With grades array" << endl;
        name = n;
        numSubjects = subjects;
        grades = new int[numSubjects];
        // Copy grades
        for(int i = 0; i < numSubjects; i++) {
            grades[i] = gradeArray[i];
        }
    }

    ~StudentGrades() {
        if(grades != nullptr) {
            delete[] grades;
        }
    }

    void display() {
        cout << "Student: " << name << ", Subjects: " << numSubjects;
        if(grades != nullptr && numSubjects > 0) {
            cout << ", Grades: ";
            for(int i = 0; i < numSubjects; i++) {
                cout << grades[i] << " ";
            }
        }
        cout << endl;
    }
};

// ============================================================================
// MAIN FUNCTION - DEMONSTRATING ALL EXAMPLES
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: BANK ACCOUNT - MULTIPLE CONSTRUCTORS ===" << endl;
    
    BankAccount acc1;
    acc1.display();
    
    BankAccount acc2("Rahul");
    acc2.display();
    
    BankAccount acc3("Priya", 10000);
    acc3.display();
    
    BankAccount acc4("Amit", 50000, "Current");
    acc4.display();
    
    BankAccount acc5("Sita", 123456789, 100000, "Salary");
    acc5.display();
    
    cout << "\n=== EXAMPLE 2: RECTANGLE - DIFFERENT TYPES ===" << endl;
    
    Rectangle r1;           // Default
    r1.display();
    
    Rectangle r2(5.5);      // Double square
    r2.display();
    
    Rectangle r3(10, 20);   // Rectangle
    r3.display();
    
    Rectangle r4(7);        // Integer square (different type)
    r4.display();
    
    cout << "\n=== EXAMPLE 3: EMPLOYEE - REAL WORLD ===" << endl;
    
    Employee e1(101, "Rohan");
    e1.display();
    
    Employee e2(102, "Sneha", "Engineering");
    e2.display();
    
    Employee e3(103, "Vikram", "Sales", 45000);
    e3.display();
    
    Employee e4(104, "Anita", "HR", 55000, "anita.sharma@company.com");
    e4.display();
    
    cout << "\n=== EXAMPLE 4: STUDENT GRADES - WITH ARRAYS ===" << endl;
    
    StudentGrades s1("Arjun");
    s1.display();
    
    StudentGrades s2("Kavya", 5);
    s2.display();
    
    // Using different name to avoid conflict with class member variable 'grades'
    int studentGrades[] = {85, 90, 78, 92, 88};
    StudentGrades s3("Raj", studentGrades, 5);
    s3.display();
    
    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Constructor overloading = Multiple constructors with different parameters" << endl;
    cout << "2. Provides flexibility in object creation" << endl;
    cout << "3. Compiler selects constructor based on arguments" << endl;
    cout << "4. Can have different parameter counts or types" << endl;
    cout << "5. Commonly used in real-world applications for flexibility" << endl;
    
    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is constructor overloading?
 * A1: Having multiple constructors in a class with different parameter lists
 *     (different number, types, or order of parameters). The compiler chooses
 *     the appropriate constructor based on the arguments provided.
 * 
 * Q2: How does the compiler distinguish between overloaded constructors?
 * A2: By the number, type, and order of parameters (function signature).
 *     Return type is NOT considered (constructors have no return type anyway).
 * 
 * Q3: Can you overload constructor with same number and type of parameters?
 * A3: No! If two constructors have the same signature (number and types of
 *     parameters), it's a compilation error. Parameter names don't matter.
 * 
 * Q4: What is the advantage of constructor overloading?
 * A4: - Flexibility in object creation
 *     - Different initialization options for different use cases
 *     - Makes code more intuitive and user-friendly
 *     - Reduces the need for setter methods after construction
 * 
 * Q5: What's the difference between constructor overloading and default parameters?
 * A5: Constructor overloading: Multiple separate constructors
 *     Default parameters: Single constructor with optional parameters
 *     Overloading gives more control and can have completely different logic.
 * 
 * Q6: Can constructor overloading cause ambiguity?
 * A6: Yes! Example:
 *     Rectangle(double side)
 *     Rectangle(int side)
 *     Calling: Rectangle r(5);  // Ambiguous! Could match either
 *     Solution: Be explicit: Rectangle r(5.0) or Rectangle r((int)5)
 * 
 * Q7: Is constructor overloading polymorphism?
 * A7: Yes, it's compile-time polymorphism (static polymorphism). The compiler
 *     decides which constructor to call at compile time based on arguments.
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. CONSTRUCTOR SIGNATURE:
 *    - Number of parameters
 *    - Type of parameters
 *    - Order of parameters
 *    (Parameter names are NOT part of signature)
 * 
 * 2. AMBIGUITY CASES:
 *    - Type conversions can cause ambiguity
 *    - Default parameters can overlap with other constructors
 * 
 * 3. BEST PRACTICES:
 *    - Keep constructors simple and focused
 *    - Use initialization lists (covered in next part)
 *    - Consider using default parameters instead of many constructors
 *    - For many parameters, consider Builder pattern
 * 
 * 4. COPY CONSTRUCTOR IS ALSO OVERLOADING:
 *    - Copy constructor is just another overloaded constructor
 *    - It has special syntax: ClassName(const ClassName&)
 */
