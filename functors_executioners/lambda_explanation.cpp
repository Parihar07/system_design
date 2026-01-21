#include <iostream>
#include <vector>
#include <algorithm>
#include <functional> // For std::function

/*
 * ARCHITECTURAL NOTE: Lambda Closures and Memory
 *
 * A lambda function is not just a function pointer. The compiler generates a unique, unnamed functor (a struct/class with an overloaded `operator()`).
 * This functor, called a "closure," holds the state of any captured variables.
 *
 * The size of the lambda object itself depends directly on the size of the variables it captures.
 * - Capture by value `[x]`: The closure contains a member variable that is a copy of `x`.
 * - Capture by reference `[&y]`: The closure contains a member variable that is a pointer (or reference) to `y`.
 *
 * This design allows lambdas to be lightweight and efficient. When passed to an algorithm like `std::count_if`,
 * the closure object is typically passed by value, and the `operator()` may be inlined by the compiler,
 * resulting in performance comparable to a hand-written loop.
 */

// --- Capture by Value vs. Capture by Reference ---
void lambda_capture_memory_layout() {
    int x = 10; // Resides on the stack frame of this function
    int y = 20; // Resides on the stack frame of this function

    /*
     * Lambda 'value_capture_lambda' Memory Layout:
     * Captures 'x' by value. The compiler generates a closure type roughly like:
     *
     * struct __unnamed_lambda_1 {
     *     int x_copy; // A copy of the original 'x'
     *
     *     auto operator()() const {
     *         std::cout << "Inside lambda: x_copy = " << x_copy << '\n';
     *     }
     * };
     *
     * The object `value_capture_lambda` on the stack will have a size of `sizeof(int)`.
     */
    auto value_capture_lambda = [x]() {
        // This 'x' is a member of the closure, not the original 'x' on the stack.
        std::cout << "Inside value-capture lambda: x = " << x << '\n';
    };
    value_capture_lambda();

    /*
     * Lambda 'ref_capture_lambda' Memory Layout:
     * Captures 'y' by reference. The compiler generates a closure type roughly like:
     *
     * struct __unnamed_lambda_2 {
     *     int* y_ptr; // A pointer/reference to the original 'y'
     *
     *     auto operator()() {
     *         std::cout << "Inside ref-capture lambda: *y_ptr = " << *y_ptr << '\n';
     *         *y_ptr = 30; // Modifies the original 'y' via the pointer
     *     }
     * };
     *
     * The object `ref_capture_lambda` on the stack will have a size of `sizeof(int*)`.
     */
    auto ref_capture_lambda = [&y]() {
        // This 'y' is an alias for the original 'y' on the stack.
        // Modifying it here has side effects outside the lambda.
        y = 30;
    };

    std::cout << "Before calling reference-capture lambda: y = " << y << '\n';
    ref_capture_lambda();
    std::cout << "After calling reference-capture lambda: y = " << y << '\n';
}

// --- Lambda with STL Algorithms ---
void lambda_stl_example() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    // The lambda here is a predicate.
    // A small, efficient closure object is created and passed to std::count_if.
    // The compiler will likely inline the call to the lambda's operator(),
    // making this loop extremely efficient, avoiding the overhead of a full function call.
    int even_count = std::count_if(v.begin(), v.end(), [](int n) {
        return n % 2 == 0;
    });

    std::cout << "Vector has " << even_count << " even numbers." << '\n';
}

// --- Lambda vs. Function Pointer ---
// A function to be pointed to. It has a fixed address in the .text section.
int add(int a, int b) { return a + b; }

void lambda_vs_function_pointer() {
    // fp is a simple pointer, 8 bytes on x86-64. It points to code.
    int (*fp)(int, int) = add;

    // non_capturing_lambda is an object of a compiler-generated closure type.
    // Since it captures nothing, the object is zero-size (or 1 byte due to language rules).
    // The compiler can implicitly convert a non-capturing lambda to a function pointer.
    int (*lambda_as_fp)(int, int) = [](int a, int b) { return a + b; };

    std::cout << "Result from function pointer: " << fp(5, 10) << '\n';
    std::cout << "Result from lambda-as-function-pointer: " << lambda_as_fp(5, 10) << '\n';

    // A capturing lambda cannot be converted to a function pointer because it has state (data members).
    // You must use std::function to create a type-erased wrapper.
    int offset = 100;
    std::function<int(int, int)> capturing_lambda = [offset](int a, int b) {
        return a + b + offset;
    };
    // The std::function object on the stack manages the closure, which may be allocated on the heap
    // if it's too large (Small Buffer Optimization). This introduces overhead compared to a direct lambda call.
    std::cout << "Result from capturing lambda via std::function: " << capturing_lambda(5, 10) << '\n';
}


int main() {
    std::cout << "--- Lambda Capture Memory Layout ---" << '\n';
    lambda_capture_memory_layout();

    std::cout << "\n--- Lambda with STL Algorithms ---" << '\n';
    lambda_stl_example();

    std::cout << "\n--- Lambda vs. Function Pointer Internals ---" << '\n';
    lambda_vs_function_pointer();

    return 0;
}