// Example focus:
// 1) Wrap lower-level parse errors with actionable context (which input failed and why).
// 2) Ensure parsing consumes the whole string; trailing characters are treated as errors.
// 3) Use catch-all + `throw;` to log locally while preserving the original exception type/message.
// 4) Show caller-side catches receiving enriched messages from both direct throws and rethrows.
#include <iostream>
#include <stdexcept>
#include <string>

int parse_int(const std::string &s)
{
    size_t idx = 0;
    int value = 0;
    try
    {
        value = std::stoi(s, &idx);
    }
    catch (const std::invalid_argument &)
    {
        // Wrap and rethrow with context
        throw std::invalid_argument("parse_int: not a number: '" + s + "'");
    }
    catch (const std::out_of_range &)
    {
        throw std::out_of_range("parse_int: out of range: '" + s + "'");
    }
    if (idx != s.size())
    {
        throw std::invalid_argument("parse_int: trailing characters in '" + s + "'");
    }
    return value;
}

// Demonstrates logging-and-rethrow while preserving the original exception type/message.
void demo_rethrow()
{
    try
    {
        parse_int("abc");
    }
    catch (...)
    {
        std::cerr << "demo_rethrow: caught something, rethrowing...\n";
        throw; // preserve original dynamic type
    }
}

// Drives the demo: shows wrapped parse errors and preserved rethrow behavior.
int main()
{
    std::cout << "-- Exceptions from functions & rethrow --\n";
    try
    {
        std::cout << "parse_int('123') = " << parse_int("123") << "\n";
        std::cout << "parse_int('123x') = " << parse_int("123x") << "\n"; // throws
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught: " << e.what() << "\n";
    }

    try
    {
        demo_rethrow();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Outer catch after rethrow: " << e.what() << "\n";
    }
}
