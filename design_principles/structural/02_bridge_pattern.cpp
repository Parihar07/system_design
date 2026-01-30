/**
 * BRIDGE PATTERN
 *
 * Intent: Decouple abstraction from implementation so both can vary independently.
 * Separates "what" (abstraction) from "how" (implementation).
 *
 * Problem: You have multiple dimensions of variation, leading to combinatorial explosion.
 * Solution: Split into two hierarchies - abstraction and implementation, linked by composition.
 *
 * Real-world analogy: Remote control (abstraction) works with any TV brand (implementation)
 *
 * SOLID relation:
 * - SRP: Separate abstraction concerns from implementation concerns
 * - OCP: Extend either hierarchy independently without modifying the other
 * - DIP: Both hierarchies depend on abstract interfaces
 */

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// EXAMPLE 1: Shape Rendering Problem
// ============================================================================

// PROBLEM: Class explosion with inheritance
// ---
// You have 3 shapes (Circle, Square, Triangle) and need to render them in:
// - Vector format (PDF, SVG)
// - Raster format (PNG, BMP)
//
// WITHOUT Bridge pattern, you'd need:
// ✗ CircleVector, CircleRaster
// ✗ SquareVector, SquareRaster
// ✗ TriangleVector, TriangleRaster
// Total: 3 shapes × 2 rendering modes = 6 classes!
//
// Add 1 more rendering mode (3D)? Now 9 classes!
// Add 1 more shape? 12 classes!
// This is N*M combinatorial explosion!
//
// Problems:
// ✗ Maintenance nightmare - small change affects many classes
// ✗ Difficult to extend - adding feature requires touching everywhere
// ✗ Tight coupling between shape and rendering
// ✗ Violates SRP - class handles both shape logic AND rendering

// ============================================================================
// SOLUTION: Bridge Pattern - Two independent hierarchies
// ============================================================================
//
// SOLUTION: Decouple shape from rendering
// ---
// Key insight: Create TWO hierarchies:
// 1. WHAT (Abstraction): Shape types (Circle, Square)
// 2. HOW (Implementation): Rendering modes (Vector, Raster)
//
// Connect them with a bridge (simple reference), allowing:
// - Each shape works with ANY renderer
// - Each renderer works with ANY shape
// - Add new shapes WITHOUT touching renderers
// - Add new renderers WITHOUT touching shapes
//
// With Bridge (N shapes + M renderers = N + M classes):
// ✓ 3 shapes + 2 renderers = 5 classes total (not 6!)
// ✓ Add 3D renderer? Now 6 classes (not 9!)
// ✓ Add Triangle? Now 6 classes (not 8!)
// ✓ Shapes change independently from renderers

// Implementation hierarchy (HOW it's done)
class IRenderer
{
public:
    virtual ~IRenderer() = default;
    virtual void renderCircle(double x, double y, double radius) = 0;
    virtual void renderSquare(double x, double y, double side) = 0;
};

// Concrete implementations
class VectorRenderer : public IRenderer
{
public:
    void renderCircle(double x, double y, double radius) override
    {
        std::cout << "[Vector] Drawing circle at (" << x << "," << y
                  << ") radius=" << radius << "\n";
    }

    void renderSquare(double x, double y, double side) override
    {
        std::cout << "[Vector] Drawing square at (" << x << "," << y
                  << ") side=" << side << "\n";
    }
};

class RasterRenderer : public IRenderer
{
public:
    void renderCircle(double x, double y, double radius) override
    {
        std::cout << "[Raster] Rendering circle pixels at (" << x << "," << y
                  << ") radius=" << radius << "\n";
    }

    void renderSquare(double x, double y, double side) override
    {
        std::cout << "[Raster] Rendering square pixels at (" << x << "," << y
                  << ") side=" << side << "\n";
    }
};

// Abstraction hierarchy (WHAT it does)
class Shape
{
protected:
    std::unique_ptr<IRenderer> renderer_; // Bridge to implementation
    double x_, y_;

public:
    Shape(std::unique_ptr<IRenderer> renderer, double x, double y)
        : renderer_(std::move(renderer)), x_(x), y_(y) {}

    virtual ~Shape() = default;
    virtual void draw() = 0;
    virtual void resize(double factor) = 0;
};

// Refined abstractions
class Circle : public Shape
{
private:
    double radius_;

public:
    Circle(std::unique_ptr<IRenderer> renderer, double x, double y, double radius)
        : Shape(std::move(renderer), x, y), radius_(radius) {}

    void draw() override
    {
        renderer_->renderCircle(x_, y_, radius_);
    }

    void resize(double factor) override
    {
        radius_ *= factor;
        std::cout << "Circle resized to radius=" << radius_ << "\n";
    }
};

class Square : public Shape
{
private:
    double side_;

public:
    Square(std::unique_ptr<IRenderer> renderer, double x, double y, double side)
        : Shape(std::move(renderer), x, y), side_(side) {}

    void draw() override
    {
        renderer_->renderSquare(x_, y_, side_);
    }

    void resize(double factor) override
    {
        side_ *= factor;
        std::cout << "Square resized to side=" << side_ << "\n";
    }
};

// ============================================================================
// EXAMPLE 2: Message System Problem
// ============================================================================

// PROBLEM: Coupling different concerns
// ---
// You need to send messages urgently or on a schedule via email/SMS/Slack
//
// Without Bridge (combinatorial nightmare):
// ✗ UrgentEmail, ScheduledEmail
// ✗ UrgentSMS, ScheduledSMS
// ✗ UrgentSlack, ScheduledSlack
// Total: 2 message types × 3 transports = 6 classes!
//
// And each class has duplicate scheduling logic AND duplicate sending logic!
//
// What happens when:
// ✗ You add a 4th transport (Telegram)? 8 classes now
// ✗ You add retry logic to URGENT messages? Must modify 3 classes
// ✗ You fix a bug in scheduling? Must fix in 3 classes
//
// Problems:
// ✗ Code duplication (message logic repeated everywhere)
// ✗ DRY violation - same feature in many places
// ✗ Tight coupling makes changes propagate everywhere
// ✗ Testing nightmare - so many combinations
//
// SOLUTION: Bridge separates message TYPE from TRANSPORT
// ---
// Message hierarchy (what) - handles message behavior
// Transport hierarchy (how) - handles delivery
// Bridge: Message holds reference to Transport
//
// Now 2 types + 3 transports = 5 classes (not 6!)
// ✓ Add Telegram? 6 classes total
// ✓ Fix retry logic once, applies to all message types
// ✓ Add new message type? Automatically works with all transports// Implementation interface
class IMessageSender
{
public:
    virtual ~IMessageSender() = default;
    virtual void sendMessage(const std::string &recipient, const std::string &body) = 0;
};

// Concrete implementations
class EmailSender : public IMessageSender
{
public:
    void sendMessage(const std::string &recipient, const std::string &body) override
    {
        std::cout << "[Email] To: " << recipient << "\n";
        std::cout << "[Email] Body: " << body << "\n";
    }
};

class SMSSender : public IMessageSender
{
public:
    void sendMessage(const std::string &recipient, const std::string &body) override
    {
        std::cout << "[SMS] To: " << recipient << "\n";
        std::cout << "[SMS] Message: " << body << " (char count: " << body.size() << ")\n";
    }
};

class SlackSender : public IMessageSender
{
public:
    void sendMessage(const std::string &recipient, const std::string &body) override
    {
        std::cout << "[Slack] @" << recipient << ": " << body << "\n";
    }
};

// Abstraction hierarchy
class Message
{
protected:
    IMessageSender &sender_; // Bridge
    std::string recipient_;

public:
    Message(IMessageSender &sender, const std::string &recipient)
        : sender_(sender), recipient_(recipient) {}

    virtual ~Message() = default;
    virtual void send() = 0;
};

// Refined abstractions
class UrgentMessage : public Message
{
private:
    std::string content_;

public:
    UrgentMessage(IMessageSender &sender, const std::string &recipient, const std::string &content)
        : Message(sender, recipient), content_(content) {}

    void send() override
    {
        std::string urgentContent = "[URGENT] " + content_;
        sender_.sendMessage(recipient_, urgentContent);
    }
};

class ScheduledMessage : public Message
{
private:
    std::string content_;
    std::string scheduleTime_;

public:
    ScheduledMessage(IMessageSender &sender, const std::string &recipient,
                     const std::string &content, const std::string &time)
        : Message(sender, recipient), content_(content), scheduleTime_(time) {}

    void send() override
    {
        std::string scheduledContent = "[Scheduled for " + scheduleTime_ + "] " + content_;
        sender_.sendMessage(recipient_, scheduledContent);
    }
};

// ============================================================================
// EXAMPLE 3: Device abstraction with platform implementation
// ============================================================================

// Implementation interface
class IDevice
{
public:
    virtual ~IDevice() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void setVolume(int level) = 0;
};

// Concrete device
class TV : public IDevice
{
private:
    bool on_ = false;
    int volume_ = 50;

public:
    void turnOn() override
    {
        on_ = true;
        std::cout << "[TV] Turned ON\n";
    }
    void turnOff() override
    {
        on_ = false;
        std::cout << "[TV] Turned OFF\n";
    }
    void setVolume(int level) override
    {
        volume_ = level;
        std::cout << "[TV] Volume set to " << volume_ << "\n";
    }
};

class Radio : public IDevice
{
private:
    bool on_ = false;
    int volume_ = 30;

public:
    void turnOn() override
    {
        on_ = true;
        std::cout << "[Radio] Turned ON\n";
    }
    void turnOff() override
    {
        on_ = false;
        std::cout << "[Radio] Turned OFF\n";
    }
    void setVolume(int level) override
    {
        volume_ = level;
        std::cout << "[Radio] Volume set to " << volume_ << "\n";
    }
};

// Abstraction (Remote control)
class RemoteControl
{
protected:
    IDevice &device_;

public:
    RemoteControl(IDevice &device) : device_(device) {}
    virtual ~RemoteControl() = default;

    void power() { device_.turnOn(); }
    void volumeUp() { device_.setVolume(60); } // Simplified
};

// Refined abstraction
class AdvancedRemote : public RemoteControl
{
public:
    AdvancedRemote(IDevice &device) : RemoteControl(device) {}

    void mute()
    {
        std::cout << "[Advanced Remote] Muting device\n";
        device_.setVolume(0);
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== BRIDGE PATTERN DEMO ===\n";

    // Shapes with different renderers
    std::cout << "\n--- Shapes with Renderers ---\n";
    {
        auto vectorCircle = std::make_unique<Circle>(
            std::make_unique<VectorRenderer>(), 10, 20, 5);
        vectorCircle->draw();
        vectorCircle->resize(2.0);
        vectorCircle->draw();

        auto rasterSquare = std::make_unique<Square>(
            std::make_unique<RasterRenderer>(), 30, 40, 10);
        rasterSquare->draw();
        rasterSquare->resize(0.5);
        rasterSquare->draw();
    }

    // Messages with different senders
    std::cout << "\n--- Messages with Different Senders ---\n";
    {
        EmailSender emailSender;
        SMSSender smsSender;
        SlackSender slackSender;

        UrgentMessage urgentEmail(emailSender, "boss@company.com", "Server is down!");
        urgentEmail.send();

        UrgentMessage urgentSMS(smsSender, "+1234567890", "Meeting in 5 min");
        urgentSMS.send();

        ScheduledMessage scheduledSlack(slackSender, "team-channel",
                                        "Deploy at midnight", "00:00");
        scheduledSlack.send();
    }

    // Remote controls with devices
    std::cout << "\n--- Remote Controls with Devices ---\n";
    {
        TV tv;
        Radio radio;

        RemoteControl tvRemote(tv);
        tvRemote.power();
        tvRemote.volumeUp();

        AdvancedRemote radioRemote(radio);
        radioRemote.power();
        radioRemote.mute();
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Two independent hierarchies: Abstraction and Implementation\n";
    std::cout << "2. Abstraction contains a reference to Implementation (bridge)\n";
    std::cout << "3. Both can vary independently without combinatorial explosion\n";
    std::cout << "4. Prefer composition over inheritance for flexibility\n";
    std::cout << "5. Plan upfront vs Adapter (retrofit)\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Avoid permanent binding between abstraction and implementation
 * - Need to extend both abstraction and implementation independently
 * - Changes in implementation shouldn't affect clients
 * - Multiple dimensions of variation (N abstractions × M implementations)
 * - Share implementation among multiple objects (hide from client)
 *
 * BENEFITS:
 * - OCP: Extend abstraction and implementation independently
 * - SRP: Separate high-level logic from platform details
 * - Hide implementation details from client
 * - Prevents class explosion
 *
 * DRAWBACKS:
 * - More complex design upfront
 * - Extra level of indirection
 * - May be overkill for simple scenarios
 *
 * COMPARISON:
 * - Bridge: Designed upfront, two hierarchies vary independently
 * - Adapter: Retrofit, make incompatible interfaces work
 * - Strategy: One hierarchy, algorithm variation only
 * - Abstract Factory: Creates objects, doesn't separate hierarchies
 */
