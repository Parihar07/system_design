// Example focus:
// 1) Show the std::exception class hierarchy split into logic_error vs runtime_error families.
// 2) Demonstrate catching specific derived types first, then their base classes, then std::exception.
// 3) Emphasize handler ordering: most specific before general to avoid premature catches.
#include <iostream>
#include <stdexcept>

// Throws from the logic_error branch (deterministic, precondition-style issues).
void logic_demo() { throw std::domain_error("Domain error: invalid input domain"); }

// Throws from the runtime_error branch (environment/runtime conditions).
void runtime_demo() { throw std::runtime_error("Runtime error: failed operation"); }

// Drives the hierarchy demo and catch ordering.
int main()
{
    std::cout << "-- std::exception hierarchy --\n";

    try
    {
        logic_demo();
    }
    catch (const std::domain_error &e)
    {
        std::cerr << "Caught domain_error specifically: " << e.what() << "\n";
    }

    try
    {
        runtime_demo();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Caught runtime_error specifically: " << e.what() << "\n";
    }

    // Order matters: more specific before general
    try
    {
        logic_demo();
    }
    catch (const std::logic_error &e)
    {
        std::cerr << "Caught logic_error (base of domain_error): " << e.what() << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught std::exception: " << e.what() << "\n";
    }
}
