#include <iostream>
#include <string>
using namespace std;

/*
 * CONSTRUCTORS - BASICS
 * =====================
 * A constructor is a special member function that:
 * 1. Has the SAME NAME as the class
 * 2. NO RETURN TYPE (not even void)
 * 3. Automatically called when an object is created
 * 4. Used to INITIALIZE objects
 * 
 * INTERVIEW QUESTION: What is a constructor?
 * ANSWER: A constructor is a special member function with the same name as the
 * class, has no return type, and is automatically invoked when an object is created.
 * Its primary purpose is to initialize the object's data members.
 */

class Student {
private:
    string name;
    int age;
    int rollNumber;

public:
    // DEFAULT CONSTRUCTOR (No parameters)
    // If you don't write any constructor, compiler provides a default one
    Student() {
        cout << "Default Constructor Called!" << endl;
        name = "Unknown";
        age = 0;
        rollNumber = 0;
    }

    // Display function
    void display() {
        cout << "Name: " << name << ", Age: " << age << ", Roll: " << rollNumber << endl;
    }
};

// Example without explicit constructor
class Box {
private:
    int length;
    int width;

public:
    // NO CONSTRUCTOR DEFINED
    // Compiler will provide a default constructor that does nothing
    
    void setDimensions(int l, int w) {
        length = l;
        width = w;
    }

    void display() {
        cout << "Length: " << length << ", Width: " << width << endl;
    }
};

int main() {
    cout << "=== BASIC CONSTRUCTOR DEMO ===" << endl;
    cout << endl;
    
    // Creating object - Constructor is called automatically
    cout << "Creating Student object s1:" << endl;
    Student s1;
    s1.display();
    
    cout << "\n=== WITHOUT EXPLICIT CONSTRUCTOR ===" << endl;
    Box b1;  // Compiler-provided default constructor called (does nothing)
    // length and width have garbage values
    cout << "Box b1 before initialization: ";
    // b1.display();  // Uncommenting this will show garbage values
    
    b1.setDimensions(10, 20);
    cout << "Box b1 after initialization: ";
    b1.display();
    
    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Constructor has same name as class" << endl;
    cout << "2. No return type (not even void)" << endl;
    cout << "3. Called automatically when object is created" << endl;
    cout << "4. If not defined, compiler provides default constructor" << endl;
    cout << "5. Used to initialize object's data members" << endl;
    
    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What happens if you don't define a constructor?
 * A1: The compiler automatically provides a default constructor that does nothing.
 * 
 * Q2: Can a constructor have a return type?
 * A2: No, constructors cannot have a return type, not even void.
 * 
 * Q3: When is a constructor called?
 * A3: A constructor is called automatically when an object of the class is created.
 * 
 * Q4: How many constructors can a class have?
 * A4: A class can have multiple constructors (constructor overloading).
 * 
 * Q5: Can constructors be private?
 * A5: Yes, private constructors are used in Singleton pattern and factory methods.
 */
