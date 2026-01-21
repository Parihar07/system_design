#include <iostream>
#include <system_error>
#include <filesystem>

namespace fs = std::filesystem;

/*
 * This program demonstrates TWO ERROR HANDLING PHILOSOPHIES in C++:
 *
 * 1. ERROR CODE STYLE:
 *    - Functions take an error_code parameter by reference
 *    - Never throws exceptions
 *    - Returns success/failure as bool or similar
 *    - Caller must manually check return value and error code
 *    - Pros: No exception overhead, predictable control flow, performance-friendly
 *
 * 2. EXCEPTION STYLE:
 *    - Functions throw exceptions on failure
 *    - No explicit error checking needed in happy path
 *    - Error handling separated from main logic via try/catch
 *    - Exceptions propagate up the call stack automatically
 *    - Pros: Cleaner code, automatic error propagation, clear separation of concerns
 *
 * Modern C++ libraries (like <filesystem>) often provide BOTH styles.
 * Choose based on: performance needs, code clarity, error handling strategy.
 */

int main()
{
    std::cout << "-- Error codes vs exceptions (filesystem) --\n";

    // ========== ERROR CODE STYLE ==========
    // Pass error_code by reference to capture error information
    // Function NEVER throws - all errors reported via error_code
    std::error_code ec;
    bool ok = fs::remove("no_such_file.txt", ec);

    // Must manually check return value and error code
    std::cout << "remove (error_code) ok=" << ok << ", ec.value=" << ec.value()
              << ", message='" << ec.message() << "'\n";
    // Note: ok=false means file didn't exist, but this is NOT an error condition
    // ec.value=0 means "no error", just "operation had no effect"

    // ========== EXCEPTION STYLE ==========
    // No error_code parameter - function throws on errors
    // Cleaner call site, but need try/catch for error handling
    try
    {
        bool removed = fs::remove("no_such_file.txt");
        std::cout << "remove (exception) call succeeded (no throw), file removed=" << removed << "\n";
        // IMPORTANT: remove() doesn't throw if file doesn't exist - returns false (0) instead
        // It only throws for actual errors (permission denied, I/O errors, invalid path, etc.)
    }
    catch (const fs::filesystem_error &e)
    {
        // Specific exception type for filesystem operations
        // Provides path information and detailed error messages
        std::cerr << "filesystem_error: path='" << e.path1().string() << "' msg='" << e.what() << "'\n";
    }
    catch (const std::exception &e)
    {
        // Catch-all for other exceptions
        std::cerr << "std::exception: " << e.what() << "\n";
    }

    std::cout << "\n-- To see an actual exception, try removing a directory that requires permissions --\n";

    return 0;
}
  