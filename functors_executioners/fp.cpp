#include <iostream>
using namespace std;

// =============================
// Function Pointer Examples
// =============================
// Demonstrates basic usage and callback pattern with function pointers in C++

// 1. Basic function pointer usage
void greet()
{
    cout << "Hello from greet!\n";
}

// 2. Function to be used as a callback
int doSomething(int x)
{
    cout << "Doing something\n";
    return 21 + x;
}

// 3. Callback function: takes a function pointer and an int
void callbackFunction(int (*fp)(int), int x)
{
    cout << "processing function\n";
    cout << fp(x) << '\n';
}

// same thing can be achieved using typedef
// Legacy C-style:
typedef int (*funPtr)(int, int);

// Modern C++ style (Recommended):
// using funPtr = int(*)(int, int);

int add(int x, int y)
{
    return x + y;
}

int subtract(int x, int y)
{
    return x - y;
}

int compute(int a, int b, funPtr fp = add)
{
    return fp(a, b);
}

int main()
{

    // --- Basic function pointer usage ---
    void (*funcPtr)();
    funcPtr = greet;
    funcPtr();

    // --- Callback pattern with function pointer ---
    callbackFunction(doSomething, 10);

    // Typedef example
    cout << "Addition : " << compute(55, 20, add) << endl;
    // now we can use this same call function with various operations
    cout << "Subtraction : " << compute(55, 20, subtract) << endl;
    // when we have a default param assigned, there will be default behaviour
    cout << "compute : " << compute(34, 76) << endl;

    // --- Array of function pointers ---
    cout << "\nArray of function pointers example:\n";
    // Define three static functions for demonstration
    struct StaticOps
    {
        static void op1() { cout << "Operation 1\n"; }
        static void op2() { cout << "Operation 2\n"; }
        static void op3() { cout << "Operation 3\n"; }
    };
    void (*ops[3])();
    ops[0] = StaticOps::op1;
    ops[1] = StaticOps::op2;
    ops[2] = StaticOps::op3;
    for (int i = 0; i < 3; ++i)
    {
        ops[i]();
    }

    return 0;
}