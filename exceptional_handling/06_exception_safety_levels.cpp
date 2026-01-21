#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/*
 * Goal: Demonstrate two levels of exception safety guarantees:
 * 1. Basic guarantee: If exception occurs, object is in a valid but possibly modified state
 * 2. Strong guarantee: If exception occurs, object state is completely unchanged (rollback)
 */

// Simulate an operation that can throw - used to test exception paths
struct MaybeThrow
{
    bool should_throw;
    explicit MaybeThrow(bool t) : should_throw(t) {}
    void operator()() const
    {
        if (should_throw)
            throw std::runtime_error("simulated failure");
    }
};

// Container demonstrating different exception safety guarantees
struct StringBag
{
    std::vector<std::string> data;

    // BASIC GUARANTEE: Object remains valid, but state may be partially modified
    // - If exception occurs, the string was already added to data
    // - State is consistent but different from before the call
    // - Suitable when rollback is expensive or unnecessary
    void add_basic(const std::string &s, const MaybeThrow &op)
    {
        data.push_back(s); // mutate original data immediately
        op();              // may throw; but data is already modified!
        // continue work...
    }

    // STRONG GUARANTEE: All-or-nothing (transactional/atomic behavior)
    // - If exception occurs, object state is completely unchanged
    // - If no exception, changes are committed
    // - Pattern: copy-modify-swap (or copy-and-swap idiom)
    void add_strong(const std::string &s, const MaybeThrow &op)
    {
        auto snapshot = data;  // 1. Create copy of current state
        snapshot.push_back(s); // 2. Modify the copy only
        op();                  // 3. Risky operation - may throw; original data still intact!
        data.swap(snapshot);   // 4. Commit: swap only if we got here (no throw)
    }
};

int main()
{
    std::cout << "-- Exception safety: basic vs strong --\n";
    StringBag bag;

    // TEST 1: Basic guarantee - exception occurs but data was already modified
    try
    {
        bag.add_basic("alpha", MaybeThrow(true)); // will throw after adding "alpha"
    }
    catch (const std::exception &e)
    {
        std::cerr << "add_basic failed: " << e.what() << "\n";
    }
    // Result: "alpha" was added before exception, so size = 1 (state modified)
    std::cout << "Size after add_basic failure: " << bag.data.size() << " (modified)\n";

    // TEST 2: Strong guarantee - exception occurs but original data is unchanged
    try
    {
        bag.add_strong("beta", MaybeThrow(true)); // will throw but won't modify original data
    }
    catch (const std::exception &e)
    {
        std::cerr << "add_strong failed: " << e.what() << "\n";
    }
    // Result: operation rolled back completely, size still = 1 (state unchanged)
    std::cout << "Size after add_strong failure: " << bag.data.size() << " (unchanged)\n";

    // TEST 3: Success path - no exception thrown
    bag.add_strong("gamma", MaybeThrow(false)); // won't throw, commits successfully
    std::cout << "Final contents:";
    for (const auto &s : bag.data)
        std::cout << ' ' << s; // Should show: alpha gamma (beta was never added)
    std::cout << "\n";

    // Summary: Basic guarantee leaves partial modifications, Strong guarantee ensures atomicity
}
