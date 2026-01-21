#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

/*
 * Goal: Demonstrate exception handling with async tasks and type-erased exception pointers
 *
 * Key concepts:
 * 1. std::exception_ptr - type-safe way to store and rethrow any exception
 * 2. std::async - automatically captures and propagates exceptions from background threads
 * 3. std::current_exception() - capture active exception without knowing its type
 * 4. std::rethrow_exception() - throw a stored exception
 */

// Background task that will throw an exception
int background()
{
    throw std::runtime_error("background task failed");
}

// Demonstrate manual exception capture using std::exception_ptr
// Purpose: Show how to store an exception without knowing its type
void capture_exception(std::exception_ptr &eptr)
{
    try
    {
        throw std::logic_error("captured logic error");
    }
    catch (...)
    {
        // std::current_exception() captures the currently active exception
        // Returns a std::exception_ptr (type-safe pointer to exception)
        eptr = std::current_exception();
        // eptr now holds the logic_error, can be rethrown elsewhere
    }
}

int main()
{
    std::cout << "-- Exceptions with std::async and exception_ptr --\n";

    // TEST 1: std::async automatically propagates exceptions
    // When a background task throws, the exception is stored in the future
    // and rethrown when we call future::get()
    auto fut = std::async(std::launch::async, background);
    try
    {
        fut.get(); // Blocks until task completes; rethrows if task threw
    }
    catch (const std::exception &e)
    {
        std::cerr << "async caught: " << e.what() << "\n";
    }

    // TEST 2: Manual exception capture and rethrow
    // Purpose: Show how exception_ptr enables deferred exception handling
    std::exception_ptr eptr; // Start with null pointer
    capture_exception(eptr); // Function captures exception inside eptr
    try
    {
        if (eptr) // Check if exception was stored
        {
            // std::rethrow_exception() rethrows the stored exception
            // Works in a different scope/thread than where it was caught
            std::rethrow_exception(eptr);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "rethrow_exception caught: " << e.what() << "\n";
    }

    // Summary:
    // - std::async transparently handles exceptions (stores in future)
    // - std::exception_ptr allows type-erased exception storage
    // - Useful for inter-thread exception propagation
}
