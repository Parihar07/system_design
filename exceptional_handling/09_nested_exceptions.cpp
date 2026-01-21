#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

/*
 * Goal: Demonstrate nested exceptions - preserving the original error while adding context
 *
 * Key concepts:
 * 1. std::throw_with_nested() - throw a new exception while preserving the original as nested
 * 2. std::rethrow_if_nested() - retrieve the nested exception if one exists
 * 3. Exception chains - show the full path of what went wrong at each layer
 *
 * Real-world use: When a low-level error occurs and gets caught by intermediate layers,
 * each layer can add context (e.g., "disk read failed" -> "cache layer error" -> "service unavailable")
 * allowing debuggers to see the complete error chain instead of just the final exception.
 */

// LAYER 1 (Deepest): Low-level operation that fails
void low_level()
{
    throw std::runtime_error("disk read failed");
}

// LAYER 2 (Middle): Catches low-level error and adds context
void mid_level()
{
    try
    {
        low_level();
    }
    catch (...)
    {
        // std::throw_with_nested() throws a NEW exception while preserving the ORIGINAL
        // Original: "disk read failed"
        // New: "cache layer error" (thrown with original as nested)
        // This allows higher layers to know the root cause
        std::throw_with_nested(std::runtime_error("cache layer error"));
    }
}

// LAYER 3 (Highest): Catches mid-level error and adds another layer of context
void high_level()
{
    try
    {
        mid_level();
    }
    catch (...)
    {
        // Again, preserve the previous exceptions (which includes cache error + original disk error)
        // New context: "service unavailable"
        std::throw_with_nested(std::runtime_error("service unavailable"));
    }
}

// Recursively print the exception chain with indentation
// Depth parameter shows how deep we are in the chain (for pretty-printing)
void print_nested(const std::exception &e, int depth = 0)
{
    // Print current exception with indentation
    std::cerr << std::string(depth * 2, ' ') << "- " << e.what() << "\n";
    try
    {
        // std::rethrow_if_nested() rethrows the nested exception if one exists
        // If no nested exception, this does nothing
        std::rethrow_if_nested(e);
    }
    catch (const std::exception &nested)
    {
        // Recursively print the next exception in the chain (with increased depth)
        print_nested(nested, depth + 1);
    }
    catch (...)
    {
        // no further nested info
    }
}

int main()
{
    std::cout << "-- Nested exceptions --\n";
    try
    {
        // Call the highest level, which calls mid, which calls low
        // Exception chain: low_level throws -> mid_level catches and wraps -> high_level catches and wraps
        high_level();
    }
    catch (const std::exception &e)
    {
        // Caught the outermost exception: "service unavailable"
        // But it contains the full chain of what happened
        std::cerr << "Exception chain:\n";
        print_nested(e); // Recursively print all exceptions in the chain
    }

    // OUTPUT EXPECTED:
    // Exception chain:
    // - service unavailable
    //   - cache layer error
    //     - disk read failed
    //
    // Benefits: Developers can see the complete error path for better debugging
}
