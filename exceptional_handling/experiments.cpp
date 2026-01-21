#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// Helper function that throws an exception for division by zero
int safeDivide(int a, int b)
{
    if (b == 0)
    {
        throw runtime_error("Division by zero!");
    }
    return a / b;
}

int main()
{
    cout << "trying out the exceptions..\n";

    // IMPORTANT: Integer division by zero does NOT throw an exception in C++
    // It causes UNDEFINED BEHAVIOR (crash, garbage value, or continues)
    // To catch exceptions, we must EXPLICITLY throw them

    try
    {
        int i = safeDivide(10, 2);
        cout << "No problem: " << i << endl;

        // int er_th = safeDivide(10, 0); // This THROWS an exception
        // cout << "This won't execute after throw: " << er_th << endl;

        // IMPORTANT: Out-of-bounds array access does NOT throw exceptions in C++
        // It's UNDEFINED BEHAVIOR - might crash, corrupt memory, or work "fine"
        // C++ does NOT check array bounds automatically!
        int a[5];
        a[8] = 20; // Undefined behavior - no exception thrown!
        // The catch(...) below will NOT catch this because no exception is raised

        // To demonstrate exception catching, use vector with .at() which throws:
        vector<int> v = {1, 2, 3, 4, 5};
        v.at(8) = 20; // This THROWS out_of_range exception!
    }
    catch (...)
    {
        // Catch-all for ANY other type
        cout << "Caught unknown exception - analyzing what it is...\n";

        // NESTED try-catch: Re-throw and identify specific types
        try
        {
            throw; // Re-throw the caught exception
        }
        catch (const runtime_error &e)
        {
            // Specific catch for runtime_error
            cout << "Caught runtime_error: " << e.what() << endl;
        }
        catch (const out_of_range &e)
        {
            cout << "  -> It's out_of_range: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "  -> It's a std::exception: " << e.what() << endl;
        }
        catch (...)
        {
            cout << "  -> It's not even a std::exception type\n";
        }
    }

    cout << "Program continues after handling exception.\n";
    return 0;
}