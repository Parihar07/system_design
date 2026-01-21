#include <iostream>
#include <stdexcept>

/*
 * Goal: Demonstrate two concepts:
 * 1. noexcept specifier - declare whether a function can throw exceptions
 * 2. Function try blocks - catch exceptions in constructor initializer lists
 */

// noexcept SPECIFIER: Promises this function will NOT throw exceptions
// - If it somehow throws anyway, std::terminate is called (program crash)
// - Allows compiler optimizations (no need to generate exception handling code)
// - Useful for move constructors, destructors, and performance-critical code
void f() noexcept
{
    // guaranteed not to throw (or will call std::terminate)
}

// noexcept(false) explicitly says this function CAN throw
// - This is the default behavior for most functions
// - Written explicitly here to contrast with noexcept
void g() noexcept(false)
{
    throw std::runtime_error("g throws");
}

// FUNCTION TRY BLOCK: Special syntax to catch exceptions from initializer list
struct Widget
{
    int *p;
    // Function try block syntax: 'try' comes before initializer list
    // Purpose: Catch exceptions that occur during member initialization
    // Normal try-catch can't catch initializer list exceptions
    Widget()
    try : p(new int[10]) // Initializer list - if 'new' throws, catch block handles it
    {
        std::cout << "Widget ctor body\n";
        throw std::runtime_error("construction failed"); // Simulate failure
    }
    catch (...) // Catches exceptions from BOTH initializer list AND constructor body
    {
        std::cerr << "Widget ctor caught exception; cleaning up\n";
        delete[] p; // Manual cleanup needed if exception happens
        throw;      // MUST rethrow - constructors auto-rethrow even without this
    }
    // Destructor marked noexcept - destructors should NEVER throw
    ~Widget() noexcept { delete[] p; }
};

int main()
{
    std::cout << "-- noexcept & function try blocks --\n";

    // noexcept OPERATOR: Compile-time check if expression is noexcept
    // IMPORTANT: f() and g() are NOT actually called here!
    // The compiler just checks their noexcept specification
    std::cout << "noexcept(f()): " << noexcept(f()) << "\n"; // prints 1 (true)
    std::cout << "noexcept(g()): " << noexcept(g()) << "\n"; // prints 0 (false)

    // TEST 1: Call g() which throws (even though we checked noexcept status above)
    try
    {
        g(); // Actually calls g() - will throw runtime_error
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught from g: " << e.what() << "\n";
    }

    // TEST 2: Widget construction with function try block
    try
    {
        Widget w; // Constructor throws, function try block catches and cleans up
    }
    catch (const std::exception &e)
    {
        std::cerr << "Widget construction failed: " << e.what() << "\n";
    }

    // Summary:
    // - noexcept(expr) = compile-time check, doesn't call function
    // - noexcept specifier = runtime contract (throws = terminate)
    // - Function try blocks = catch initializer list exceptions
}
