#include <iostream>
#include <memory>
#include <stdexcept>

// Utility that logs when it is created/destroyed so we can see unwinding order.
struct Tracer
{
    const char *name;
    explicit Tracer(const char *n) : name(n) { std::cout << "Construct " << name << "\n"; }
    ~Tracer() { std::cout << "Destruct  " << name << "\n"; }
};

void work()
{
    Tracer a{"A"};                       // first object on the stack
    std::unique_ptr<int> p(new int(42)); // heap resource managed via RAII
    Tracer b{"B"};                       // second object on the stack
    std::cout << "About to throw...\n";
    throw std::runtime_error("boom"); // trigger unwinding; everything above is cleaned
    Tracer c{"C"};                    // not executed because of the throw
}

int main()
{
    std::cout << "-- Stack unwinding & RAII --\n";
    // Goal: show that local objects and RAII-managed resources are destroyed in reverse
    // order when an exception propagates (stack unwinding).
    try
    {
        work();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught: " << e.what() << "\n";
    }
    std::cout << "Note: RAII cleaned resources automatically during unwinding.\n";
}
