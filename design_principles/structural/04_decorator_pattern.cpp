/**
 * DECORATOR PATTERN
 *
 * Intent: Attach additional responsibilities to an object dynamically.
 * Provides flexible alternative to subclassing for extending functionality.
 *
 * Problem: Subclassing for every feature combination leads to class explosion.
 * Solution: Wrap object with decorator objects that add behavior.
 *
 * Real-world analogy: Coffee with add-ons (milk, sugar, whipped cream) - each adds cost/behavior
 *
 * SOLID relation:
 * - OCP: Add new decorators without modifying existing classes
 * - SRP: Each decorator has single responsibility
 * - LSP: Decorators substitute for component
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Coffee Shop Problem
// ============================================================================

// PROBLEM: Inheritance explosion with feature combinations
// ---
// Coffee base: $2
// Add milk: +$0.50
// Add sugar: +$0.20
// Add whipped cream: +$0.70
// Add caramel: +$0.60
//
// Without Decorator (using inheritance):
// Classes needed:
// ✗ Coffee (base)
// ✗ CoffeeWithMilk
// ✗ CoffeeWithSugar
// ✗ CoffeeWithMilkAndSugar
// ✗ CoffeeWithMilkAndWhippedCream
// ✗ CoffeeWithMilkSugarWhipped
// ✗ CoffeeWithMilkSugarWhippedCaramel
// ... exponential explosion! With N features = 2^N classes!
//
// With just 4 add-ons: 2^4 = 16 required classes!
// With 10 add-ons: 2^10 = 1024 required classes!
//
// Problems:
// ✗ Class explosion - maintenance nightmare
// ✗ Code duplication - each combination redefines base methods
// ✗ Hard to maintain - fix in base means propagating to all combinations
// ✗ Not flexible - can only have predefined combinations
// ✗ Runtime changes impossible - must know at compile time
// ✗ Violates OCP and DRY principles
//
// SOLUTION: Decorator Pattern - Wrap instead of inherit
// ---
// Key insight: Instead of creating classes for every combination,
// create DECORATORS that wrap and enhance objects!
//
// Solution:
// 1. Base class: Coffee with simple implementation
// 2. Decorators: MilkDecorator, SugarDecorator, etc.
// 3. Stack decorators: wrap one decorator with another
//
// Now for 4 add-ons: 1 base + 4 decorators = 5 classes total (not 16!)
// For 10 add-ons: 1 base + 10 decorators = 11 classes (not 1024!)
//
// Example with Decorators:
//   auto coffee = std::make_unique<SimpleCoffee>();
//   coffee = std::make_unique<MilkDecorator>(std::move(coffee));
//   coffee = std::make_unique<SugarDecorator>(std::move(coffee));
//   coffee = std::make_unique<CaramelDecorator>(std::move(coffee));
//   // Cost: $2 + $0.50 + $0.20 + $0.60 = $3.30 (dynamically calculated!)
//
// Benefits:
// ✓ No class explosion - add new decorator, not new class
// ✓ DRY - each decorator focused on ONE responsibility
// ✓ Composable - combine any decorators in any order
// ✓ Runtime flexibility - decide at runtime what to add
// ✓ Open/Closed Principle - extend without modifying
// ✓ Order matters - coffee then caramel differs from caramel then coffee

// Component interface
class Coffee
{
public:
    virtual ~Coffee() = default;
    virtual std::string getDescription() const = 0;
    virtual double getCost() const = 0;
};

// Concrete component
class SimpleCoffee : public Coffee
{
public:
    std::string getDescription() const override
    {
        return "Simple Coffee";
    }

    double getCost() const override
    {
        return 2.0;
    }
};

// Base decorator
class CoffeeDecorator : public Coffee
{
protected:
    std::unique_ptr<Coffee> coffee_;

public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> coffee)
        : coffee_(std::move(coffee)) {}

    std::string getDescription() const override
    {
        return coffee_->getDescription();
    }

    double getCost() const override
    {
        return coffee_->getCost();
    }
};

// Concrete decorators
class MilkDecorator : public CoffeeDecorator
{
public:
    explicit MilkDecorator(std::unique_ptr<Coffee> coffee)
        : CoffeeDecorator(std::move(coffee)) {}

    std::string getDescription() const override
    {
        return coffee_->getDescription() + " + Milk";
    }

    double getCost() const override
    {
        return coffee_->getCost() + 0.5;
    }
};

class SugarDecorator : public CoffeeDecorator
{
public:
    explicit SugarDecorator(std::unique_ptr<Coffee> coffee)
        : CoffeeDecorator(std::move(coffee)) {}

    std::string getDescription() const override
    {
        return coffee_->getDescription() + " + Sugar";
    }

    double getCost() const override
    {
        return coffee_->getCost() + 0.2;
    }
};

class WhippedCreamDecorator : public CoffeeDecorator
{
public:
    explicit WhippedCreamDecorator(std::unique_ptr<Coffee> coffee)
        : CoffeeDecorator(std::move(coffee)) {}

    std::string getDescription() const override
    {
        return coffee_->getDescription() + " + Whipped Cream";
    }

    double getCost() const override
    {
        return coffee_->getCost() + 0.7;
    }
};

class CaramelDecorator : public CoffeeDecorator
{
public:
    explicit CaramelDecorator(std::unique_ptr<Coffee> coffee)
        : CoffeeDecorator(std::move(coffee)) {}

    std::string getDescription() const override
    {
        return coffee_->getDescription() + " + Caramel";
    }

    double getCost() const override
    {
        return coffee_->getCost() + 0.6;
    }
};

// ============================================================================
// EXAMPLE 2: Data stream decorators (I/O streams pattern)
// ============================================================================

class DataStream
{
public:
    virtual ~DataStream() = default;
    virtual void write(const std::string &data) = 0;
    virtual std::string read() = 0;
};

class FileStream : public DataStream
{
private:
    std::string buffer_;
    std::string filename_;

public:
    explicit FileStream(const std::string &filename) : filename_(filename) {}

    void write(const std::string &data) override
    {
        buffer_ = data;
        std::cout << "[FileStream] Writing to " << filename_ << ": " << data << "\n";
    }

    std::string read() override
    {
        std::cout << "[FileStream] Reading from " << filename_ << "\n";
        return buffer_;
    }
};

class StreamDecorator : public DataStream
{
protected:
    std::unique_ptr<DataStream> stream_;

public:
    explicit StreamDecorator(std::unique_ptr<DataStream> stream)
        : stream_(std::move(stream)) {}
};

class CompressionDecorator : public StreamDecorator
{
public:
    explicit CompressionDecorator(std::unique_ptr<DataStream> stream)
        : StreamDecorator(std::move(stream)) {}

    void write(const std::string &data) override
    {
        std::string compressed = compress(data);
        std::cout << "[Compression] Compressed: " << data.size()
                  << " -> " << compressed.size() << " bytes\n";
        stream_->write(compressed);
    }

    std::string read() override
    {
        std::string compressed = stream_->read();
        std::string decompressed = decompress(compressed);
        std::cout << "[Compression] Decompressed: " << compressed.size()
                  << " -> " << decompressed.size() << " bytes\n";
        return decompressed;
    }

private:
    std::string compress(const std::string &data)
    {
        return "[COMPRESSED:" + data + "]";
    }

    std::string decompress(const std::string &data)
    {
        // Simplified decompression
        if (data.find("[COMPRESSED:") == 0)
        {
            return data.substr(12, data.size() - 13);
        }
        return data;
    }
};

class EncryptionDecorator : public StreamDecorator
{
public:
    explicit EncryptionDecorator(std::unique_ptr<DataStream> stream)
        : StreamDecorator(std::move(stream)) {}

    void write(const std::string &data) override
    {
        std::string encrypted = encrypt(data);
        std::cout << "[Encryption] Encrypted data\n";
        stream_->write(encrypted);
    }

    std::string read() override
    {
        std::string encrypted = stream_->read();
        std::string decrypted = decrypt(encrypted);
        std::cout << "[Encryption] Decrypted data\n";
        return decrypted;
    }

private:
    std::string encrypt(const std::string &data)
    {
        return "[ENCRYPTED:" + data + "]";
    }

    std::string decrypt(const std::string &data)
    {
        if (data.find("[ENCRYPTED:") == 0)
        {
            return data.substr(11, data.size() - 12);
        }
        return data;
    }
};

class BufferingDecorator : public StreamDecorator
{
private:
    std::string buffer_;
    static constexpr size_t BUFFER_SIZE = 100;

public:
    explicit BufferingDecorator(std::unique_ptr<DataStream> stream)
        : StreamDecorator(std::move(stream)) {}

    void write(const std::string &data) override
    {
        buffer_ += data;
        if (buffer_.size() >= BUFFER_SIZE)
        {
            flush();
        }
        else
        {
            std::cout << "[Buffering] Buffered " << buffer_.size() << " bytes\n";
        }
    }

    std::string read() override
    {
        return stream_->read();
    }

    void flush()
    {
        if (!buffer_.empty())
        {
            std::cout << "[Buffering] Flushing " << buffer_.size() << " bytes\n";
            stream_->write(buffer_);
            buffer_.clear();
        }
    }
};

// ============================================================================
// EXAMPLE 3: Notification system
// ============================================================================

class Notifier
{
public:
    virtual ~Notifier() = default;
    virtual void send(const std::string &message) = 0;
};

class BasicNotifier : public Notifier
{
public:
    void send(const std::string &message) override
    {
        std::cout << "[Basic] Notification: " << message << "\n";
    }
};

class NotifierDecorator : public Notifier
{
protected:
    std::unique_ptr<Notifier> notifier_;

public:
    explicit NotifierDecorator(std::unique_ptr<Notifier> notifier)
        : notifier_(std::move(notifier)) {}

    void send(const std::string &message) override
    {
        notifier_->send(message);
    }
};

class EmailDecorator : public NotifierDecorator
{
public:
    explicit EmailDecorator(std::unique_ptr<Notifier> notifier)
        : NotifierDecorator(std::move(notifier)) {}

    void send(const std::string &message) override
    {
        notifier_->send(message);
        sendEmail(message);
    }

private:
    void sendEmail(const std::string &message)
    {
        std::cout << "[Email] Sent email: " << message << "\n";
    }
};

class SMSDecorator : public NotifierDecorator
{
public:
    explicit SMSDecorator(std::unique_ptr<Notifier> notifier)
        : NotifierDecorator(std::move(notifier)) {}

    void send(const std::string &message) override
    {
        notifier_->send(message);
        sendSMS(message);
    }

private:
    void sendSMS(const std::string &message)
    {
        std::cout << "[SMS] Sent SMS: " << message << "\n";
    }
};

class SlackDecorator : public NotifierDecorator
{
public:
    explicit SlackDecorator(std::unique_ptr<Notifier> notifier)
        : NotifierDecorator(std::move(notifier)) {}

    void send(const std::string &message) override
    {
        notifier_->send(message);
        sendSlack(message);
    }

private:
    void sendSlack(const std::string &message)
    {
        std::cout << "[Slack] Sent Slack message: " << message << "\n";
    }
};

// ============================================================================
// Demonstration
// ============================================================================

void demonstrateCoffee()
{
    std::cout << "\n--- Coffee Shop Example ---\n";

    // Simple coffee
    auto coffee1 = std::make_unique<SimpleCoffee>();
    std::cout << coffee1->getDescription() << " = $" << coffee1->getCost() << "\n";

    // Coffee with milk
    auto coffee2 = std::make_unique<MilkDecorator>(
        std::make_unique<SimpleCoffee>());
    std::cout << coffee2->getDescription() << " = $" << coffee2->getCost() << "\n";

    // Fancy coffee: milk + sugar + whipped cream + caramel
    auto coffee3 = std::make_unique<CaramelDecorator>(
        std::make_unique<WhippedCreamDecorator>(
            std::make_unique<SugarDecorator>(
                std::make_unique<MilkDecorator>(
                    std::make_unique<SimpleCoffee>()))));
    std::cout << coffee3->getDescription() << " = $" << coffee3->getCost() << "\n";
}

void demonstrateStreams()
{
    std::cout << "\n--- Data Stream Example ---\n";

    // File with compression and encryption
    auto stream = std::make_unique<EncryptionDecorator>(
        std::make_unique<CompressionDecorator>(
            std::make_unique<FileStream>("data.bin")));

    std::cout << "\nWriting:\n";
    stream->write("Sensitive data that needs compression and encryption");

    std::cout << "\nReading:\n";
    std::string data = stream->read();
    std::cout << "Final data: " << data << "\n";
}

void demonstrateNotifications()
{
    std::cout << "\n--- Notification System Example ---\n";

    // Multi-channel notification
    auto notifier = std::make_unique<SlackDecorator>(
        std::make_unique<SMSDecorator>(
            std::make_unique<EmailDecorator>(
                std::make_unique<BasicNotifier>())));

    notifier->send("Server alert: High CPU usage detected!");
}

int main()
{
    std::cout << "=== DECORATOR PATTERN DEMO ===\n";

    demonstrateCoffee();
    demonstrateStreams();
    demonstrateNotifications();

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Wrap objects to add responsibilities dynamically\n";
    std::cout << "2. Alternative to subclassing - more flexible\n";
    std::cout << "3. Decorators can be stacked (composed)\n";
    std::cout << "4. Each decorator adds ONE responsibility (SRP)\n";
    std::cout << "5. Same interface as component (transparent to client)\n";
    std::cout << "6. Order of decoration matters!\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Add responsibilities to objects dynamically and transparently
 * - Responsibilities can be withdrawn
 * - Extension by subclassing is impractical (class explosion)
 * - Need to add features independently in any combination
 *
 * BENEFITS:
 * - More flexible than static inheritance
 * - Avoids feature-laden classes high in hierarchy
 * - Pay-as-you-go approach (add only what you need)
 * - Easy to add new decorators (OCP)
 * - Responsibilities can be added/removed at runtime
 *
 * DRAWBACKS:
 * - Many small objects in the system
 * - Decorator and component aren't identical (type checking issues)
 * - Harder to debug (multiple layers of wrapping)
 * - Order dependency in decoration chain
 *
 * DESIGN CONSIDERATIONS:
 * - Keep component interface simple
 * - Make decorators transparent (conform to component interface)
 * - Don't depend on object identity (decorated ≠ original)
 * - Consider providing decorator base class for common functionality
 *
 * COMPARISON:
 * - Decorator: Adds responsibilities, same interface, can be nested
 * - Adapter: Changes interface to make incompatible things work
 * - Proxy: Same interface, controls access (doesn't add responsibility)
 * - Composite: Structural composition, not behavioral augmentation
 * - Strategy: Changes algorithm/behavior, not add layers
 */
