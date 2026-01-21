// Example focus:
// 1) Define a custom exception type deriving from std::exception with a stored message.
// 2) Mark the constructor explicit to avoid accidental implicit conversions from std::string.
// 3) Override what() as noexcept to expose the stored message safely.
// 4) Show caller-side catch ordering: specific custom type first, then std::exception fallback.
#include <exception>
#include <iostream>
#include <string>

class MyError : public std::exception
{
    std::string msg_;

public:
    // explicit prevents implicit std::string -> MyError conversions; forces intentional construction.
    explicit MyError(std::string msg) : msg_(std::move(msg)) {}
    // Return the stored message; noexcept is required by std::exception contract.
    const char *what() const noexcept override { return msg_.c_str(); }
};

// Throws our custom exception with context.
void risky() { throw MyError("MyError: something went wrong"); }

// Drives the custom exception demo: specific catch first, then generic fallback.
int main()
{
    std::cout << "-- Custom exception type --\n";
    try
    {
        risky();
    }
    catch (const MyError &e)
    {
        std::cerr << "Caught MyError: " << e.what() << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught std::exception: " << e.what() << "\n";
    }
}
