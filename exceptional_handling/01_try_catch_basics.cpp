#include <iostream>
#include <exception>
using namespace std;

int divide(int a, int b)
{
    if (b == 0)
    {
        throw invalid_argument("division by zero");
    }
    return a / b;
}

int main()
{
    cout << "-- Basic try/catch/throw --\n";
    try
    {
        cout << "10 / 2 = " << divide(10, 2) << "\n";
        cout << "10 / 0 = " << divide(10, 0) << "\n"; // throws
        cout << "This line won't execute after throw\n";
    }
    // IMPORTANT: Only ONE catch block executes per exception!
    // The first matching catch block handles it, then skips the rest.
    catch (const invalid_argument &e)
    {
        // Most specific catch - matches invalid_argument exactly
        // When divide(10,0) throws invalid_argument, THIS executes
        // The exception and (...) blocks below are SKIPPED
        cerr << "Caught invalid_argument: " << e.what() << "\n";
    }
    catch (const exception &e)
    {
        // More general catch - matches all std::exception types
        // This would catch runtime_error, logic_error, etc.
        // But NOT invalid_argument (already caught above)
        cerr << "Caught exception: " << e.what() << "\n";
    }
    catch (...)
    {
        // Catch-all - ALWAYS goes last
        // Catches ANY type (int, string, custom types, etc.)
        cerr << "Caught unknown exception\n";
    }

    // Demonstrate catch-all with non-exception type
    try
    {
        throw 42; // Throwing an int (not a std::exception type)
    }
    catch (const exception &e)
    {
        // This does NOT match int - exception catch only works for std::exception types
        // This block is SKIPPED
        cerr << "Will not catch int: " << e.what() << "\n";
    }
    catch (...)
    {
        // Catch-all matches ANYTHING, including primitive types like int
        // THIS block executes
        cerr << "Caught non-exception type with catch-all (...)\n";
    }

    cout << "Program continues after handling exceptions.\n";
    return 0;
}
