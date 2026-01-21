#include <iostream>
using namespace std;

/*
 * UNDERSTANDING EXPLICIT vs IMPLICIT CONSTRUCTORS
 * ================================================
 * This file explains WHY we need the 'explicit' keyword.
 * 
 * KEY CONCEPT: Single-parameter constructors can act as CONVERSION OPERATORS
 * Without 'explicit', C++ automatically converts types using your constructor!
 */

// ============================================================================
// WITHOUT EXPLICIT - IMPLICIT CONVERSION HAPPENS
// ============================================================================

class Distance {
private:
    int meters;

public:
    // This constructor takes ONE parameter (int)
    // WITHOUT 'explicit', C++ treats this as:
    // "I can convert an int to a Distance object automatically"
    Distance(int m) : meters(m) {
        cout << "  Distance constructor called with: " << m << endl;
    }

    void display() const {
        cout << "  Distance: " << meters << " meters" << endl;
    }
};

// Function expecting a Distance object
void processDistance(Distance d) {
    cout << "  Processing distance..." << endl;
    d.display();
}

// ============================================================================
// WITH EXPLICIT - PREVENTS IMPLICIT CONVERSION
// ============================================================================

class DistanceExplicit {
private:
    int meters;

public:
    // 'explicit' tells compiler:
    // "Do NOT automatically convert int to DistanceExplicit"
    // "User must explicitly create the object"
    explicit DistanceExplicit(int m) : meters(m) {
        cout << "  DistanceExplicit constructor called with: " << m << endl;
    }

    void display() const {
        cout << "  Distance: " << meters << " meters" << endl;
    }
};

void processDistanceExplicit(DistanceExplicit d) {
    cout << "  Processing distance..." << endl;
    d.display();
}

// ============================================================================
// REAL-WORLD EXAMPLE: WHY IMPLICIT CONVERSION CAN BE DANGEROUS
// ============================================================================

class BankAccount {
private:
    double balance;

public:
    // WITHOUT explicit - DANGEROUS!
    BankAccount(double bal) : balance(bal) {
        cout << "  Account created with balance: ₹" << balance << endl;
    }

    void display() const {
        cout << "  Balance: ₹" << balance << endl;
    }
};

class SafeBankAccount {
private:
    double balance;

public:
    // WITH explicit - SAFE!
    explicit SafeBankAccount(double bal) : balance(bal) {
        cout << "  Account created with balance: ₹" << balance << endl;
    }

    void display() const {
        cout << "  Balance: ₹" << balance << endl;
    }
};

void transferMoney(BankAccount acc) {
    cout << "  Transferring money from account..." << endl;
    acc.display();
}

void transferMoneySafe(SafeBankAccount acc) {
    cout << "  Transferring money from account..." << endl;
    acc.display();
}

// ============================================================================
// MAIN FUNCTION - DEMONSTRATIONS
// ============================================================================

int main() {
    cout << "=== SCENARIO 1: IMPLICIT CONVERSION (WITHOUT explicit) ===" << endl;
    cout << "\n1. Normal object creation:" << endl;
    Distance d1(100);
    d1.display();

    cout << "\n2. IMPLICIT CONVERSION - int to Distance:" << endl;
    Distance d2 = 100;  // Looks like assignment, but actually calls constructor!
    // What happens: C++ sees "100" (int), needs Distance object
    //               Finds Distance(int) constructor
    //               Automatically converts: Distance d2 = Distance(100);
    d2.display();

    cout << "\n3. IMPLICIT CONVERSION in function call:" << endl;
    processDistance(200);  // Pass int, C++ converts to Distance automatically!
    // What happens: Function expects Distance object
    //               You passed int (200)
    //               C++ creates temporary: Distance temp(200)
    //               Calls: processDistance(temp)
    //               temp destroyed after function

    cout << "\n4. This is DANGEROUS - You might not want this behavior!" << endl;
    
    cout << "\n\n=== SCENARIO 2: EXPLICIT KEYWORD (PREVENTS implicit) ===" << endl;
    cout << "\n1. Normal object creation - WORKS:" << endl;
    DistanceExplicit d3(300);
    d3.display();

    cout << "\n2. IMPLICIT CONVERSION - BLOCKED! (Compile Error if uncommented):" << endl;
    // DistanceExplicit d4 = 400;  // ERROR! Cannot implicitly convert
    // Why error? Constructor is 'explicit', C++ won't auto-convert
    cout << "  ❌ Line commented out - would cause compile error!" << endl;
    cout << "  Error: conversion from 'int' to non-scalar type 'DistanceExplicit' requested" << endl;

    cout << "\n3. Must use EXPLICIT construction:" << endl;
    DistanceExplicit d5 = DistanceExplicit(500);  // OK - Explicit creation
    d5.display();

    cout << "\n4. IMPLICIT CONVERSION in function - BLOCKED!" << endl;
    // processDistanceExplicit(600);  // ERROR! Cannot implicitly convert
    cout << "  ❌ Line commented out - would cause compile error!" << endl;
    
    cout << "\n5. Must explicitly create object:" << endl;
    processDistanceExplicit(DistanceExplicit(700));  // OK - Explicit creation

    cout << "\n\n=== SCENARIO 3: WHY THIS MATTERS - REAL WORLD ===" << endl;
    
    cout << "\n1. WITHOUT explicit - DANGEROUS behavior:" << endl;
    BankAccount acc1(5000);
    acc1.display();
    
    cout << "\n2. Implicit conversion can cause bugs:" << endl;
    transferMoney(1000);  // DANGEROUS! Creates temporary account with ₹1000
    // What if you meant to pass account ID (1000) instead of balance?
    // Bug: Creates NEW account with ₹1000 instead of using existing account
    
    cout << "\n3. WITH explicit - SAFE behavior:" << endl;
    SafeBankAccount acc2(5000);
    acc2.display();
    
    cout << "\n4. Forces explicit intention:" << endl;
    // transferMoneySafe(2000);  // ERROR! Must be explicit
    cout << "  ❌ Cannot accidentally pass int - must create object explicitly" << endl;
    transferMoneySafe(SafeBankAccount(3000));  // OK - Clear intention

    cout << "\n\n=== KEY TAKEAWAYS ===" << endl;
    cout << "\n1. WITHOUT 'explicit':" << endl;
    cout << "   - Single-parameter constructor = automatic type conversion" << endl;
    cout << "   - int → Distance happens automatically" << endl;
    cout << "   - Can cause bugs and unexpected behavior" << endl;
    cout << "   - Less type-safe" << endl;

    cout << "\n2. WITH 'explicit':" << endl;
    cout << "   - Prevents automatic type conversion" << endl;
    cout << "   - Must explicitly create objects" << endl;
    cout << "   - More type-safe" << endl;
    cout << "   - Prevents accidental conversions and bugs" << endl;

    cout << "\n3. BEST PRACTICE:" << endl;
    cout << "   - Use 'explicit' for ALL single-parameter constructors" << endl;
    cout << "   - Unless you SPECIFICALLY want implicit conversion" << endl;
    cout << "   - Modern C++ style: explicit by default" << endl;

    cout << "\n4. WHAT HAPPENS INTERNALLY:" << endl;
    cout << "   Distance d = 100;" << endl;
    cout << "   ↓ Compiler sees this as:" << endl;
    cout << "   Distance d = Distance(100);  // Implicit conversion" << endl;
    cout << "   ↓ Creates temporary Distance object" << endl;
    cout << "   ↓ Copies it to d (or optimized away)" << endl;

    cout << "\n5. WHY DOES C++ ALLOW THIS?" << endl;
    cout << "   - Historical reasons" << endl;
    cout << "   - Sometimes convenient (e.g., std::string s = \"hello\";)" << endl;
    cout << "   - But can cause bugs if not careful" << endl;
    cout << "   - That's why 'explicit' keyword was added!" << endl;

    return 0;
}

/*
 * DETAILED EXPLANATION:
 * =====================
 * 
 * Q: Why does "Distance d = 100;" work without 'explicit'?
 * A: Because single-parameter constructors act as CONVERSION OPERATORS.
 *    C++ sees:
 *    1. You want a Distance object
 *    2. You provided an int (100)
 *    3. There's a constructor Distance(int)
 *    4. C++ automatically calls Distance(100) to convert int → Distance
 * 
 * Q: What exactly happens during implicit conversion?
 * A: Step by step:
 *    Distance d = 100;
 *    
 *    Step 1: Compiler sees "100" is int, d is Distance
 *    Step 2: Looks for way to convert int → Distance
 *    Step 3: Finds constructor Distance(int)
 *    Step 4: Creates temporary: Distance temp(100);
 *    Step 5: Copy/move temp to d (often optimized away)
 *    
 *    Essentially becomes: Distance d = Distance(100);
 * 
 * Q: Why is this dangerous?
 * A: Examples:
 *    - void withdraw(BankAccount acc) { }
 *    - withdraw(5000);  // Creates NEW account with ₹5000!
 *    - You probably meant account ID 5000, not new account!
 *    
 *    - Array arr = 10;  // Creates array with 10 elements
 *    - Might confuse with array index or value
 *    
 *    - Socket connection = 8080;  // Creates socket on port 8080?
 *    - Unclear intention
 * 
 * Q: When is implicit conversion useful?
 * A: Some cases where you WANT it:
 *    - std::string s = "hello";  // char* → string
 *    - std::complex<double> c = 3.14;  // double → complex
 *    - std::vector<int> v = {1, 2, 3};  // initializer_list
 *    
 *    But these are CAREFULLY designed! Default: use explicit.
 * 
 * Q: What does 'explicit' actually do?
 * A: Tells compiler: "Do NOT use this constructor for implicit conversions"
 *    - Direct initialization still works: Distance d(100); ✓
 *    - Explicit cast works: Distance d = Distance(100); ✓
 *    - Implicit conversion blocked: Distance d = 100; ✗
 *    - Function calls blocked: processDistance(100); ✗
 * 
 * Q: Does 'explicit' affect performance?
 * A: NO! It's a compile-time check only.
 *    Same code generated, just prevents accidental conversions.
 * 
 * Q: Should I use 'explicit' for multi-parameter constructors?
 * A: C++11+: Yes, you can!
 *    explicit MyClass(int a, int b) { }
 *    Prevents: MyClass obj = {1, 2};  // Blocked
 *    Requires: MyClass obj(1, 2);     // OK
 * 
 * INTERVIEW POINTS:
 * =================
 * 1. Single-parameter constructors are conversion operators by default
 * 2. 'explicit' prevents implicit type conversions
 * 3. Best practice: Use explicit unless you specifically want conversion
 * 4. Makes code more type-safe and prevents bugs
 * 5. No performance cost - compile-time only
 * 6. Modern C++ style: explicit by default
 * 
 * REMEMBER:
 * =========
 * WITHOUT explicit: Distance d = 100;        ✓ (implicit conversion)
 * WITH explicit:    Distance d = 100;        ✗ (compile error)
 * WITH explicit:    Distance d(100);         ✓ (direct initialization)
 * WITH explicit:    Distance d = Distance(100); ✓ (explicit conversion)
 */
