/**
 * ITERATOR PATTERN
 *
 * Intent: Provide a way to access elements of a collection sequentially without
 * exposing its underlying representation.
 *
 * Problem: Different collections have different structures; want uniform access
 * Solution: Define iterator interface to traverse any collection
 *
 * Real-world analogy: TV remote with channel up/down - iterate through channels
 *
 * SOLID relation:
 * - SRP: Separation of collection from iteration logic
 * - OCP: Add new collections/iterators without modifying existing ones
 * - DIP: Client depends on Iterator abstraction, not concrete collection
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// EXAMPLE 1: Collection Traversal Problem
// ============================================================================

// PROBLEM: Different collections require different traversal logic
// ---
// Collections: Array, LinkedList, Tree, Graph
// Each has different internal structure:
// - Array: indexed access, O(1)
// - LinkedList: sequential access, O(n)
// - Tree: depth-first or breadth-first traversal
// - Graph: DFS, BFS, topological sort
//
// Without Iterator pattern:
// ✗ Client must know collection internal structure
// ✗ Different access code for each collection type:
//   for (int i = 0; i < array.size(); i++) { ... }
//   for (Node* n = list.head; n != null; n = n->next) { ... }
//   traverseTree(tree.root);
//   traverseGraph(graph.start);
//
// ✗ Adding new collection requires teaching clients how to traverse it
// ✗ Traversal logic scattered throughout codebase
// ✗ Hard to switch collection types (must rewrite all loops)
// ✗ Can't have multiple concurrent iterations
// ✗ Violates OCP - add collection = modify many client files
//
// Example messy code WITHOUT Iterator:
//   if (collection is Array) {
//       for (int i = 0; i < size; i++) { process(arr[i]); }
//   } else if (collection is LinkedList) {
//       for (Node* n = head; n; n = n->next) { process(n->data); }
//   } else if (collection is Tree) {
//       traverseTree(root);
//   }
//   // Duplicated everywhere - DRY violation!

// SOLUTION: Iterator Pattern - Uniform traversal
// ---
// Key insight: Hide collection structure behind Iterator
// 1. Collection provides createIterator()
// 2. Iterator provides: hasNext(), next()
// 3. Client uses Iterator without knowing collection type
//
// Benefits:
// ✓ Uniform interface for all collections
// ✓ Clients don't know collection structure
// ✓ Easy to add new collections
// ✓ Multiple simultaneous iterations (each has own iterator)
// ✓ Support different traversal strategies
// ✓ OCP - add collection without changing clients

// Data to iterate
struct Book
{
    std::string title;
    std::string author;
};

// Iterator interface
class BookIterator
{
public:
    virtual ~BookIterator() = default;
    virtual bool hasNext() const = 0;
    virtual Book next() = 0;
};

// Collection interface
class BookCollection
{
public:
    virtual ~BookCollection() = default;
    virtual std::unique_ptr<BookIterator> createIterator() = 0;
};

// Array-based collection
class ArrayBookCollection : public BookCollection
{
private:
    std::vector<Book> books_;

public:
    ArrayBookCollection()
    {
        books_.push_back({"C++ Primer", "Lippman"});
        books_.push_back({"Design Patterns", "Gang of Four"});
        books_.push_back({"Clean Code", "Martin"});
    }

    void addBook(const Book &b) { books_.push_back(b); }

    std::unique_ptr<BookIterator> createIterator() override;
};

// Array iterator
class ArrayBookIterator : public BookIterator
{
private:
    const std::vector<Book> &books_;
    size_t currentIndex_;

public:
    ArrayBookIterator(const std::vector<Book> &b) : books_(b), currentIndex_(0) {}

    bool hasNext() const override
    {
        return currentIndex_ < books_.size();
    }

    Book next() override
    {
        if (hasNext())
        {
            return books_[currentIndex_++];
        }
        return {"", ""};
    }
};

inline std::unique_ptr<BookIterator> ArrayBookCollection::createIterator()
{
    return std::make_unique<ArrayBookIterator>(books_);
}

// Linked list-based collection
class LinkedListNode
{
public:
    Book data;
    std::shared_ptr<LinkedListNode> next;

    LinkedListNode(const Book &b) : data(b), next(nullptr) {}
};

class LinkedListBookCollection : public BookCollection
{
private:
    std::shared_ptr<LinkedListNode> head_;

public:
    LinkedListBookCollection()
    {
        addBook({"C++ Primer", "Lippman"});
        addBook({"Design Patterns", "Gang of Four"});
        addBook({"Clean Code", "Martin"});
    }

    void addBook(const Book &b)
    {
        auto newNode = std::make_shared<LinkedListNode>(b);
        newNode->next = head_;
        head_ = newNode;
    }

    std::unique_ptr<BookIterator> createIterator() override;
    std::shared_ptr<LinkedListNode> getHead() const { return head_; }
};

// LinkedList iterator
class LinkedListBookIterator : public BookIterator
{
private:
    std::shared_ptr<LinkedListNode> current_;

public:
    LinkedListBookIterator(std::shared_ptr<LinkedListNode> head) : current_(head) {}

    bool hasNext() const override
    {
        return current_ != nullptr;
    }

    Book next() override
    {
        if (hasNext())
        {
            Book data = current_->data;
            current_ = current_->next;
            return data;
        }
        return {"", ""};
    }
};

inline std::unique_ptr<BookIterator> LinkedListBookCollection::createIterator()
{
    return std::make_unique<LinkedListBookIterator>(head_);
}

// ============================================================================
// EXAMPLE 2: Reverse Iterator
// ============================================================================

// PROBLEM: Need to traverse backwards
// ---
// Same collection, different traversal direction

class ReverseBookIterator : public BookIterator
{
private:
    const std::vector<Book> &books_;
    int currentIndex_;

public:
    ReverseBookIterator(const std::vector<Book> &b)
        : books_(b), currentIndex_(b.size() - 1) {}

    bool hasNext() const override
    {
        return currentIndex_ >= 0;
    }

    Book next() override
    {
        if (hasNext())
        {
            return books_[currentIndex_--];
        }
        return {"", ""};
    }
};

// ============================================================================
// EXAMPLE 3: Filtered Iterator
// ============================================================================

// PROBLEM: Need to filter while iterating
// ---
// Iterate only books by specific author

class FilteredBookIterator : public BookIterator
{
private:
    const std::vector<Book> &books_;
    std::string authorFilter_;
    size_t currentIndex_;

    void skipToNextMatch()
    {
        while (currentIndex_ < books_.size() &&
               books_[currentIndex_].author != authorFilter_)
        {
            currentIndex_++;
        }
    }

public:
    FilteredBookIterator(const std::vector<Book> &b, const std::string &author)
        : books_(b), authorFilter_(author), currentIndex_(0)
    {
        skipToNextMatch();
    }

    bool hasNext() const override
    {
        return currentIndex_ < books_.size();
    }

    Book next() override
    {
        if (hasNext())
        {
            Book result = books_[currentIndex_];
            currentIndex_++;
            skipToNextMatch();
            return result;
        }
        return {"", ""};
    }
};

// ============================================================================
// Demonstration
// ============================================================================

void printCollection(BookCollection &collection, const std::string &name)
{
    std::cout << "\n"
              << name << ":\n";
    auto iterator = collection.createIterator();
    int count = 1;
    while (iterator->hasNext())
    {
        Book book = iterator->next();
        std::cout << "  " << count++ << ". \"" << book.title << "\" by " << book.author << "\n";
    }
}

int main()
{
    std::cout << "=== ITERATOR PATTERN DEMO ===\n";

    // Array collection
    std::cout << "\n--- Array-Based Collection ---\n";
    {
        ArrayBookCollection arrayBooks;
        printCollection(arrayBooks, "Books (Array)");

        // Multiple concurrent iterators
        std::cout << "\nMultiple concurrent iterations:\n";
        auto iter1 = arrayBooks.createIterator();
        auto iter2 = arrayBooks.createIterator();

        std::cout << "Iterator 1: " << iter1->next().title << "\n";
        std::cout << "Iterator 2: " << iter2->next().title << "\n";
        std::cout << "Iterator 1: " << iter1->next().title << "\n";
    }

    // LinkedList collection with same client code!
    std::cout << "\n--- LinkedList-Based Collection (Same Client Code!) ---\n";
    {
        LinkedListBookCollection listBooks;
        printCollection(listBooks, "Books (LinkedList)");
    }

    // Array with reverse iteration
    std::cout << "\n--- Reverse Iterator ---\n";
    {
        ArrayBookCollection books;
        std::cout << "Reverse order:\n";
        auto iterator = std::make_unique<ReverseBookIterator>(
            std::vector<Book>{
                {"C++ Primer", "Lippman"},
                {"Design Patterns", "Gang of Four"},
                {"Clean Code", "Martin"}});
        int count = 1;
        while (iterator->hasNext())
        {
            Book book = iterator->next();
            std::cout << "  " << count++ << ". \"" << book.title << "\"\n";
        }
    }

    // Filtered iteration
    std::cout << "\n--- Filtered Iterator ---\n";
    {
        std::vector<Book> allBooks{
            {"C++ Primer", "Lippman"},
            {"Design Patterns", "Gang of Four"},
            {"Clean Code", "Martin"},
            {"More Effective C++", "Meyers"},
            {"Effective STL", "Meyers"}};

        std::cout << "Books by Meyers only:\n";
        auto iterator = std::make_unique<FilteredBookIterator>(allBooks, "Meyers");
        int count = 1;
        while (iterator->hasNext())
        {
            Book book = iterator->next();
            std::cout << "  " << count++ << ". \"" << book.title << "\" by " << book.author << "\n";
        }
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Uniform interface for different collections\n";
    std::cout << "2. Hide internal collection structure\n";
    std::cout << "3. Support multiple concurrent iterations\n";
    std::cout << "4. Easy to add new collections\n";
    std::cout << "5. Support different traversal strategies\n";
    std::cout << "6. Client code doesn't change when collection type changes\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Access collection elements without exposing structure
 * - Multiple traversals of same collection
 * - Different traversal strategies (forward, backward, filtered)
 * - Uniform access across different collection types
 *
 * BENEFITS:
 * - Separates collection from iteration
 * - Uniform interface for all collections
 * - Multiple concurrent iterations
 * - Support different traversal strategies
 * - OCP - add collections without modifying clients
 * - SRP - collection and iterator have separate concerns
 *
 * DRAWBACKS:
 * - More classes and indirection
 * - May not be needed for simple collections
 * - Some languages have better iteration support (range-based loops)
 *
 * VARIATIONS:
 * - Internal vs External Iterator
 * - Forward, Reverse, Bidirectional iterators
 * - Filtered, Mapped, Zipped iterators
 * - Lazy evaluation iterators
 *
 * COMPARISON:
 * - Iterator: Traverse collection sequentially
 * - Visitor: Perform operation on collection elements
 * - Composite: Access hierarchical structure
 */
