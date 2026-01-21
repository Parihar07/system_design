#include <iostream>
#include <string>
#include <cstring>
using namespace std;

/*
 * COPY CONSTRUCTOR
 * ================
 * A copy constructor is a special constructor that creates a new object as a copy
 * of an existing object.
 * 
 * SYNTAX: ClassName(const ClassName &obj)
 * 
 * INTERVIEW QUESTION: What is a copy constructor?
 * ANSWER: A copy constructor is a constructor that initializes an object using
 * another object of the same class. It takes a reference to an object of the
 * same class as a parameter.
 */

// ============================================================================
// EXAMPLE 1: COMPILER-PROVIDED COPY CONSTRUCTOR (Shallow Copy)
// ============================================================================

class Simple {
private:
    int value;
    string name;

public:
    // Parameterized Constructor
    Simple(int v, string n) {
        value = v;
        name = n;
        cout << "Parameterized Constructor called for " << name << endl;
    }

    // If you don't define copy constructor, compiler provides one
    // It does MEMBERWISE COPY (shallow copy for pointers)
    
    void display() {
        cout << "Value: " << value << ", Name: " << name << endl;
    }
};

// ============================================================================
// EXAMPLE 2: USER-DEFINED COPY CONSTRUCTOR
// ============================================================================

class Student {
private:
    int rollNo;
    string name;
    float marks;

public:
    // Parameterized Constructor
    Student(int r, string n, float m) {
        rollNo = r;
        name = n;
        marks = m;
        cout << "Parameterized Constructor called" << endl;
    }

    // Copy Constructor
    Student(const Student &s) {
        rollNo = s.rollNo;
        name = s.name;
        marks = s.marks;
        cout << "Copy Constructor called" << endl;
    }

    void display() {
        cout << "Roll: " << rollNo << ", Name: " << name << ", Marks: " << marks << endl;
    }
};

// ============================================================================
// EXAMPLE 3: SHALLOW COPY vs DEEP COPY (MOST IMPORTANT FOR INTERVIEWS!)
// ============================================================================

class ShallowCopyExample {
private:
    int *ptr;
    int size;

public:
    ShallowCopyExample(int s) {
        size = s;
        ptr = new int[size];  // Dynamic memory allocation
        for(int i = 0; i < size; i++) {
            ptr[i] = i + 1;
        }
        cout << "Constructor: Memory allocated at " << ptr << endl;
    }

    // Compiler-provided copy constructor does SHALLOW COPY
    // Both objects will point to SAME memory location
    // This causes problems when destructor is called!

    void display() {
        cout << "Array at " << ptr << ": ";
        for(int i = 0; i < size; i++) {
            cout << ptr[i] << " ";
        }
        cout << endl;
    }

    void modify(int index, int value) {
        if(index < size) {
            ptr[index] = value;
        }
    }

    ~ShallowCopyExample() {
        cout << "Destructor: Deleting memory at " << ptr << endl;
        delete[] ptr;
    }
};

class DeepCopyExample {
private:
    int *ptr;
    int size;

public:
    DeepCopyExample(int s) {
        size = s;
        ptr = new int[size];
        for(int i = 0; i < size; i++) {
            ptr[i] = i + 1;
        }
        cout << "Constructor: Memory allocated at " << ptr << endl;
    }

    // Deep Copy Constructor - Creates NEW memory
    DeepCopyExample(const DeepCopyExample &obj) {
        size = obj.size;
        ptr = new int[size];  // Allocate NEW memory
        for(int i = 0; i < size; i++) {
            ptr[i] = obj.ptr[i];  // Copy values
        }
        cout << "Copy Constructor: New memory allocated at " << ptr << endl;
    }

    void display() {
        cout << "Array at " << ptr << ": ";
        for(int i = 0; i < size; i++) {
            cout << ptr[i] << " ";
        }
        cout << endl;
    }

    void modify(int index, int value) {
        if(index < size) {
            ptr[index] = value;
        }
    }

    ~DeepCopyExample() {
        cout << "Destructor: Deleting memory at " << ptr << endl;
        delete[] ptr;
    }
};

// ============================================================================
// WHEN IS COPY CONSTRUCTOR CALLED?
// ============================================================================

class Demo {
private:
    int x;

public:
    Demo(int val) : x(val) {
        cout << "Normal Constructor" << endl;
    }

    Demo(const Demo &d) : x(d.x) {
        cout << "Copy Constructor" << endl;
    }

    void display() {
        cout << "x = " << x << endl;
    }
};

void functionByValue(Demo obj) {
    cout << "Inside function" << endl;
    obj.display();
}

Demo functionReturningObject() {
    Demo temp(100);
    return temp;  // Copy constructor may be called (or optimized away)
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== EXAMPLE 1: COMPILER-PROVIDED COPY CONSTRUCTOR ===" << endl;
    Simple s1(10, "Object1");
    Simple s2 = s1;  // Copy constructor called (compiler-provided)
    s1.display();
    s2.display();
    
    cout << "\n=== EXAMPLE 2: USER-DEFINED COPY CONSTRUCTOR ===" << endl;
    Student st1(101, "Alice", 95.5);
    Student st2 = st1;  // Copy constructor called
    st1.display();
    st2.display();
    
    cout << "\n=== EXAMPLE 3A: SHALLOW COPY PROBLEM ===" << endl;
    cout << "WARNING: This will cause double delete problem!" << endl;
    // Uncomment to see the problem:
    /*
    {
        ShallowCopyExample obj1(3);
        obj1.display();
        
        ShallowCopyExample obj2 = obj1;  // Shallow copy
        obj2.display();
        
        obj2.modify(0, 999);
        cout << "After modifying obj2:" << endl;
        obj1.display();  // obj1 is also modified! (same memory)
        obj2.display();
        
        // When this block ends, BOTH destructors try to delete SAME memory
        // This causes DOUBLE DELETE error / crash
    }
    */
    
    cout << "\n=== EXAMPLE 3B: DEEP COPY SOLUTION ===" << endl;
    {
        DeepCopyExample obj1(3);
        obj1.display();
        
        DeepCopyExample obj2 = obj1;  // Deep copy
        obj2.display();
        
        obj2.modify(0, 999);
        cout << "After modifying obj2:" << endl;
        obj1.display();  // obj1 is NOT modified (different memory)
        obj2.display();
        
        // Both destructors delete their OWN memory - No problem!
    }
    
    cout << "\n=== WHEN IS COPY CONSTRUCTOR CALLED? ===" << endl;
    
    // Case 1: Object initialized with another object
    cout << "\n1. Object initialized with another object:" << endl;
    Demo d1(10);
    Demo d2 = d1;  // Copy constructor
    Demo d3(d1);   // Copy constructor (explicit)
    
    // Case 2: Object passed by value to function
    cout << "\n2. Object passed by value to function:" << endl;
    functionByValue(d1);
    
    // Case 3: Object returned from function
    cout << "\n3. Object returned from function:" << endl;
    Demo d4 = functionReturningObject();  // May be optimized (RVO)
    
    cout << "\n=== KEY TAKEAWAYS ===" << endl;
    cout << "1. Copy constructor creates object from existing object" << endl;
    cout << "2. Syntax: ClassName(const ClassName &obj)" << endl;
    cout << "3. SHALLOW COPY: Copies pointer address (dangerous with dynamic memory)" << endl;
    cout << "4. DEEP COPY: Creates new memory and copies values (safe)" << endl;
    cout << "5. Always define copy constructor if class has pointers!" << endl;
    cout << "6. Copy constructor called in 3 situations:" << endl;
    cout << "   - Object initialization with another object" << endl;
    cout << "   - Pass by value to function" << endl;
    cout << "   - Return by value from function" << endl;
    
    return 0;
}

/*
 * INTERVIEW QUESTIONS:
 * 
 * Q1: What is the difference between shallow copy and deep copy?
 * A1: Shallow copy copies the pointer address (both objects share same memory).
 *     Deep copy creates new memory and copies the actual values.
 *     Shallow copy causes problems with dynamic memory allocation.
 * 
 * Q2: When should you define your own copy constructor?
 * A2: When the class has:
 *     - Dynamic memory allocation (pointers)
 *     - File handles
 *     - Network connections
 *     - Any resource that needs special handling
 * 
 * Q3: Why do we pass object by reference (const &) in copy constructor?
 * A3: - To avoid infinite recursion (passing by value would call copy constructor)
 *     - 'const' ensures original object is not modified
 *     - Reference avoids making another copy
 * 
 * Q4: What happens if you don't define a copy constructor?
 * A4: Compiler provides a default copy constructor that does memberwise shallow copy.
 *     This is fine for classes without pointers, but dangerous with dynamic memory.
 * 
 * Q5: Can copy constructor be private?
 * A5: Yes! Private copy constructor prevents copying of objects (useful for Singleton).
 * 
 * Q6: What is copy elision / RVO?
 * A6: Return Value Optimization - compiler optimization that eliminates unnecessary
 *     copy constructor calls when returning objects from functions.
 * 
 * Q7: What is the Rule of Three?
 * A7: If a class needs a custom destructor, copy constructor, or copy assignment
 *     operator, it probably needs all three (related to resource management).
 */

/*
 * IMPORTANT NOTES:
 * 
 * 1. ALWAYS define copy constructor if class has:
 *    - new/delete
 *    - malloc/free
 *    - File pointers
 *    - Database connections
 * 
 * 2. Copy constructor signature MUST be: ClassName(const ClassName &obj)
 *    - MUST take reference (not value)
 *    - Should be const (best practice)
 * 
 * 3. Modern C++ (C++11 onwards):
 *    - Move constructor: ClassName(ClassName &&obj)
 *    - Rule of Five: Add move constructor and move assignment
 * 
 * 4. To prevent copying:
 *    - Make copy constructor private
 *    - Or use: ClassName(const ClassName&) = delete;
 */
