/**
 * STRATEGY PATTERN
 *
 * Intent: Define a family of algorithms, encapsulate each one, and make them
 * interchangeable. Strategy lets the algorithm vary independently from clients.
 *
 * Problem: Multiple ways to do something; client needs to choose at runtime
 * Solution: Define strategy interface; each algorithm is separate class
 *
 * Real-world analogy: Multiple payment methods (credit card, cash, check)
 *
 * SOLID relation:
 * - SRP: Each strategy encapsulates one algorithm
 * - OCP: Add new strategies without modifying context
 * - DIP: Context depends on Strategy abstraction
 * - LSP: All strategies are substitutable
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// ============================================================================
// EXAMPLE 1: Sorting Algorithms
// ============================================================================

// PROBLEM: Different sorting algorithms needed at runtime
// ---
// Application may need different sort strategies:
// - Quick sort: Fast for random data, O(n log n) average
// - Merge sort: Guaranteed O(n log n), stable
// - Bubble sort: Simple but slow, O(n²)
// - Insertion sort: Good for nearly sorted data
//
// Without Strategy pattern:
// ✗ Container class hardcoded to one sort algorithm
// ✗ Adding new algorithm requires modifying container
// ✗ Can't choose algorithm at runtime
// ✗ Sorting logic mixed with container logic
// ✗ Hard to test sorting algorithms independently
// ✗ Violates SRP and OCP
//
// Example messy code WITHOUT Strategy:
//   class Sorter {
//       void sortArray(vector& arr) {
//           if (sortType == QUICK) { quickSort(arr); }
//           if (sortType == MERGE) { mergeSort(arr); }
//           if (sortType == BUBBLE) { bubbleSort(arr); }
//       }
//   };
//   // Adding new algorithm? Must edit Sorter class!

// SOLUTION: Strategy Pattern - Interchangeable algorithms
// ---
// Key insight: Each algorithm is separate class implementing common interface
// Context (container) uses strategy without knowing implementation
//
// Benefits:
// ✓ Algorithms encapsulated and isolated
// ✓ Easy to add new algorithms (OCP)
// ✓ Runtime algorithm selection
// ✓ Each algorithm independently testable
// ✓ Context doesn't need to know algorithm details
// ✓ Strategy can be changed at runtime

template <typename T>
class SortStrategy
{
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<T> &arr) = 0;
    virtual std::string getName() const = 0;
};

// Concrete strategies
template <typename T>
class QuickSortStrategy : public SortStrategy<T>
{
private:
    int comparisons_ = 0;

    void quickSort(std::vector<T> &arr, int left, int right)
    {
        if (left >= right)
            return;

        int pi = partition(arr, left, right);
        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);
    }

    int partition(std::vector<T> &arr, int left, int right)
    {
        T pivot = arr[right];
        int i = left - 1;

        for (int j = left; j < right; j++)
        {
            comparisons_++;
            if (arr[j] < pivot)
            {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[right]);
        return i + 1;
    }

public:
    void sort(std::vector<T> &arr) override
    {
        comparisons_ = 0;
        if (!arr.empty())
        {
            quickSort(arr, 0, arr.size() - 1);
        }
        std::cout << "  QuickSort: " << comparisons_ << " comparisons\n";
    }

    std::string getName() const override { return "QuickSort"; }
};

template <typename T>
class MergeSortStrategy : public SortStrategy<T>
{
private:
    int comparisons_ = 0;

    void merge(std::vector<T> &arr, int left, int mid, int right)
    {
        std::vector<T> temp;
        int i = left, j = mid + 1;

        while (i <= mid && j <= right)
        {
            comparisons_++;
            if (arr[i] <= arr[j])
            {
                temp.push_back(arr[i++]);
            }
            else
            {
                temp.push_back(arr[j++]);
            }
        }

        while (i <= mid)
            temp.push_back(arr[i++]);
        while (j <= right)
            temp.push_back(arr[j++]);

        for (int k = 0; k < static_cast<int>(temp.size()); k++)
        {
            arr[left + k] = temp[k];
        }
    }

    void mergeSort(std::vector<T> &arr, int left, int right)
    {
        if (left < right)
        {
            int mid = (left + right) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

public:
    void sort(std::vector<T> &arr) override
    {
        comparisons_ = 0;
        if (!arr.empty())
        {
            mergeSort(arr, 0, arr.size() - 1);
        }
        std::cout << "  MergeSort: " << comparisons_ << " comparisons (guaranteed O(n log n))\n";
    }

    std::string getName() const override { return "MergeSort"; }
};

// Context
template <typename T>
class SortingContext
{
private:
    std::unique_ptr<SortStrategy<T>> strategy_;
    std::vector<T> data_;

public:
    void setStrategy(std::unique_ptr<SortStrategy<T>> strategy)
    {
        strategy_ = std::move(strategy);
    }

    void addData(const T &item)
    {
        data_.push_back(item);
    }

    void sort()
    {
        if (!strategy_)
        {
            std::cout << "ERROR: No sorting strategy set\n";
            return;
        }
        std::cout << "Sorting with " << strategy_->getName() << ":\n";
        strategy_->sort(data_);
    }

    void printData()
    {
        std::cout << "  Data: ";
        for (const auto &item : data_)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
};

// ============================================================================
// EXAMPLE 2: Payment Processing
// ============================================================================

// PROBLEM: Different payment methods with different processing
// ---
// Payment methods:
// - Credit card: validate card number, process with payment gateway
// - PayPal: redirect to PayPal, receive callback
// - Bank transfer: validate account, create transfer order
// - Cryptocurrency: generate wallet address, monitor blockchain
//
// SOLUTION: Each payment method is separate strategy

class PaymentStrategy
{
public:
    virtual ~PaymentStrategy() = default;
    virtual void pay(double amount) = 0;
    virtual std::string getMethodName() const = 0;
};

class CreditCardPayment : public PaymentStrategy
{
private:
    std::string cardNumber_;
    std::string cvv_;

public:
    CreditCardPayment(const std::string &card, const std::string &cvv)
        : cardNumber_(card), cvv_(cvv) {}

    void pay(double amount) override
    {
        std::cout << "  [CreditCard] Processing $" << amount
                  << " on card " << cardNumber_.substr(cardNumber_.length() - 4) << "\n";
        std::cout << "  [CreditCard] ✓ Payment authorized\n";
    }

    std::string getMethodName() const override { return "CreditCard"; }
};

class PayPalPayment : public PaymentStrategy
{
private:
    std::string email_;

public:
    PayPalPayment(const std::string &email) : email_(email) {}

    void pay(double amount) override
    {
        std::cout << "  [PayPal] Redirecting to PayPal for $" << amount << "\n";
        std::cout << "  [PayPal] User logs in with " << email_ << "\n";
        std::cout << "  [PayPal] ✓ Payment confirmed\n";
    }

    std::string getMethodName() const override { return "PayPal"; }
};

class BitcoinPayment : public PaymentStrategy
{
private:
    std::string walletAddress_;

public:
    BitcoinPayment(const std::string &addr) : walletAddress_(addr) {}

    void pay(double amount) override
    {
        double btc = amount / 45000; // Simplified conversion
        std::cout << "  [Bitcoin] Generating QR code for " << btc << " BTC\n";
        std::cout << "  [Bitcoin] Address: " << walletAddress_ << "\n";
        std::cout << "  [Bitcoin] ✓ Transaction broadcast\n";
    }

    std::string getMethodName() const override { return "Bitcoin"; }
};

class ShoppingCart
{
private:
    std::unique_ptr<PaymentStrategy> paymentStrategy_;
    double total_ = 0.0;

public:
    void setPaymentMethod(std::unique_ptr<PaymentStrategy> strategy)
    {
        paymentStrategy_ = std::move(strategy);
    }

    void addItem(double price)
    {
        total_ += price;
    }

    void checkout()
    {
        if (!paymentStrategy_)
        {
            std::cout << "ERROR: No payment method selected\n";
            return;
        }
        std::cout << "Checkout - Total: $" << total_ << "\n";
        paymentStrategy_->pay(total_);
    }
};

// ============================================================================
// EXAMPLE 3: Compression Strategies
// ============================================================================

// PROBLEM: Need to compress data with different algorithms
// ---
// Compression options: ZIP, GZIP, BZIP2, etc.

class CompressionStrategy
{
public:
    virtual ~CompressionStrategy() = default;
    virtual void compress(const std::string &file) = 0;
    virtual std::string getName() const = 0;
};

class ZipCompression : public CompressionStrategy
{
public:
    void compress(const std::string &file) override
    {
        std::cout << "  [ZIP] Compressing " << file << " to " << file << ".zip\n";
    }
    std::string getName() const override { return "ZIP"; }
};

class GzipCompression : public CompressionStrategy
{
public:
    void compress(const std::string &file) override
    {
        std::cout << "  [GZIP] Compressing " << file << " to " << file << ".gz\n";
    }
    std::string getName() const override { return "GZIP"; }
};

class FileArchiver
{
private:
    std::unique_ptr<CompressionStrategy> strategy_;

public:
    void setStrategy(std::unique_ptr<CompressionStrategy> strategy)
    {
        strategy_ = std::move(strategy);
    }

    void archive(const std::string &file)
    {
        if (!strategy_)
        {
            std::cout << "ERROR: No compression strategy\n";
            return;
        }
        std::cout << "Archiving with " << strategy_->getName() << ":\n";
        strategy_->compress(file);
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== STRATEGY PATTERN DEMO ===\n";

    // Sorting algorithms
    std::cout << "\n--- Sorting Strategies ---\n";
    {
        SortingContext<int> sorter;
        sorter.addData(64);
        sorter.addData(34);
        sorter.addData(25);
        sorter.addData(12);
        sorter.addData(22);
        sorter.addData(11);
        sorter.addData(90);

        std::cout << "Before: ";
        sorter.printData();

        std::cout << "\nUsing QuickSort:\n";
        sorter.setStrategy(std::make_unique<QuickSortStrategy<int>>());
        sorter.sort();

        std::cout << "After: ";
        sorter.printData();

        // Switch strategy at runtime
        std::cout << "\nSwitching to MergeSort:\n";
        sorter.setStrategy(std::make_unique<MergeSortStrategy<int>>());
        sorter.sort();
    }

    // Payment methods
    std::cout << "\n--- Payment Strategies ---\n";
    {
        ShoppingCart cart;
        cart.addItem(29.99);
        cart.addItem(49.99);

        std::cout << "Payment with CreditCard:\n";
        cart.setPaymentMethod(std::make_unique<CreditCardPayment>("4532-1234-5678-9010", "123"));
        cart.checkout();

        std::cout << "\nPayment with PayPal:\n";
        cart.setPaymentMethod(std::make_unique<PayPalPayment>("user@example.com"));
        cart.checkout();

        std::cout << "\nPayment with Bitcoin:\n";
        cart.setPaymentMethod(std::make_unique<BitcoinPayment>("1A1z7agoat"));
        cart.checkout();
    }

    // Compression strategies
    std::cout << "\n--- Compression Strategies ---\n";
    {
        FileArchiver archiver;

        archiver.setStrategy(std::make_unique<ZipCompression>());
        archiver.archive("document.pdf");

        archiver.setStrategy(std::make_unique<GzipCompression>());
        archiver.archive("data.csv");
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Encapsulate family of algorithms\n";
    std::cout << "2. Make algorithms interchangeable\n";
    std::cout << "3. Choose algorithm at runtime\n";
    std::cout << "4. Each strategy is independent\n";
    std::cout << "5. Easy to add new strategies\n";
    std::cout << "6. Client doesn't depend on algorithm details\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Multiple algorithms for a task
 * - Different algorithms for different scenarios
 * - Want to avoid if-else chains
 * - Runtime algorithm selection
 * - Algorithms have common interface
 * - Want to make algorithms independently testable
 *
 * BENEFITS:
 * - Encapsulates algorithms
 * - Runtime algorithm switching
 * - Eliminates conditional statements
 * - Easy to add new algorithms (OCP)
 * - Each algorithm independently testable
 * - Algorithms vary independently of clients
 *
 * DRAWBACKS:
 * - More classes and objects
 * - Overkill for simple cases
 * - Clients must be aware of different strategies
 * - Strategy objects always created
 *
 * COMPARISON:
 * - Strategy: Client chooses algorithm
 * - State: Object changes behavior based on state
 * - Command: Encapsulates request, not algorithm
 * - Template Method: Algorithm skeleton in base class
 *
 * VARIATIONS:
 * - Strategy factory for creating strategies
 * - Strategy registry for dynamic registration
 * - Composite strategies combining multiple strategies
 */
