/**
 * FACADE PATTERN
 *
 * Intent: Provide a unified, simplified interface to a set of interfaces in a subsystem.
 * Makes the subsystem easier to use.
 *
 * Problem: Complex subsystem with many interdependent classes is hard to use.
 * Solution: Provide a high-level interface that makes subsystem easy to use.
 *
 * Real-world analogy: Restaurant menu - simplifies kitchen operations for customers
 *
 * SOLID relation:
 * - SRP: Facade handles coordination, subsystem classes handle specifics
 * - DIP: Clients depend on facade abstraction, not complex subsystem
 * - Reduces coupling between client and subsystem
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// EXAMPLE 1: Home Theater Setup Problem
// ============================================================================

// PROBLEM: Complex subsystem coordination nightmare
// ---
// Home theater system has many interdependent components:
// - Amplifier (must turn on, set volume, enable surround)
// - DVD Player (must turn on, load disc, play)
// - Projector (must turn on, set widescreen mode)
// - Lights (must dim to specific level)
// - Screen (must lower for viewing)
// - Popcorn Maker (must start and pop)
//
// WITHOUT Facade, watching a movie requires:
// ✗ lights->dim(10);
// ✗ screen->down();
// ✗ projector->on();
// ✗ projector->wideScreenMode();
// ✗ amplifier->on();
// ✗ amplifier->setSurroundSound();
// ✗ amplifier->setVolume(50);
// ✗ dvdPlayer->on();
// ✗ dvdPlayer->play("movie.iso");
// ✗ popcornMaker->on();
// ✗ popcornMaker->pop();
//
// Problems:
// ✗ Client must know ALL components and correct ordering
// ✗ One forgotten step ruins the experience
// ✗ Coupling client to internal subsystem details
// ✗ Same sequence duplicated everywhere (DRY violation)
// ✗ Adding new equipment requires changing ALL client code
// ✗ Turning off requires same complex sequence in reverse
// ✗ Error-prone and hard to maintain
// ✗ Violates Single Responsibility - client handles business logic AND coordination
//
// SOLUTION: Facade Pattern - Unified high-level interface
// ---
// Create HomeTheaterFacade that encapsulates:
// 1. ALL subsystem components
// 2. CORRECT coordination logic
// 3. SIMPLE public methods: watchMovie(), endMovie(), listenToMusic()
//
// Now client code is simple and clear:
//   HomeTheaterFacade theater;
//   theater.watchMovie("Inception");
//   // ... watch movie ...
//   theater.endMovie();
//
// Benefits:
// ✓ Simple, clear API for complex subsystem
// ✓ Client unaware of internal complexity
// ✓ All coordination logic in ONE place
// ✓ Change in equipment only affects facade
// ✓ No duplicate logic scattered everywhere
// ✓ Follows SRP - facade coordinates, subsystems handle details
// ✓ Easy to test - facade provides clear interface
// ✓ Can have multiple facades for different use cases

class Amplifier
{
public:
    void on() { std::cout << "[Amplifier] Turning on\n"; }
    void off() { std::cout << "[Amplifier] Turning off\n"; }
    void setVolume(int level)
    {
        std::cout << "[Amplifier] Setting volume to " << level << "\n";
    }
    void setSurroundSound() { std::cout << "[Amplifier] Setting surround sound\n"; }
};

class DVDPlayer
{
public:
    void on() { std::cout << "[DVD Player] Turning on\n"; }
    void off() { std::cout << "[DVD Player] Turning off\n"; }
    void play(const std::string &movie)
    {
        std::cout << "[DVD Player] Playing: " << movie << "\n";
    }
    void stop() { std::cout << "[DVD Player] Stopping\n"; }
    void eject() { std::cout << "[DVD Player] Ejecting disc\n"; }
};

class Projector
{
public:
    void on() { std::cout << "[Projector] Turning on\n"; }
    void off() { std::cout << "[Projector] Turning off\n"; }
    void wideScreenMode() { std::cout << "[Projector] Setting widescreen mode\n"; }
};

class Lights
{
public:
    void dim(int level)
    {
        std::cout << "[Lights] Dimming to " << level << "%\n";
    }
    void on() { std::cout << "[Lights] Turning on\n"; }
};

class Screen
{
public:
    void down() { std::cout << "[Screen] Lowering screen\n"; }
    void up() { std::cout << "[Screen] Raising screen\n"; }
};

class PopcornMaker
{
public:
    void on() { std::cout << "[Popcorn Maker] Turning on\n"; }
    void off() { std::cout << "[Popcorn Maker] Turning off\n"; }
    void pop() { std::cout << "[Popcorn Maker] Popping popcorn!\n"; }
};

// ============================================================================
// SOLUTION: Facade simplifies subsystem interaction
// ============================================================================

class HomeTheaterFacade
{
private:
    std::unique_ptr<Amplifier> amp_;
    std::unique_ptr<DVDPlayer> dvd_;
    std::unique_ptr<Projector> projector_;
    std::unique_ptr<Lights> lights_;
    std::unique_ptr<Screen> screen_;
    std::unique_ptr<PopcornMaker> popcorn_;

public:
    HomeTheaterFacade()
        : amp_(std::make_unique<Amplifier>()),
          dvd_(std::make_unique<DVDPlayer>()),
          projector_(std::make_unique<Projector>()),
          lights_(std::make_unique<Lights>()),
          screen_(std::make_unique<Screen>()),
          popcorn_(std::make_unique<PopcornMaker>()) {}

    void watchMovie(const std::string &movie)
    {
        std::cout << "\n=== Get ready to watch a movie ===\n";
        popcorn_->on();
        popcorn_->pop();
        lights_->dim(10);
        screen_->down();
        projector_->on();
        projector_->wideScreenMode();
        amp_->on();
        amp_->setSurroundSound();
        amp_->setVolume(50);
        dvd_->on();
        dvd_->play(movie);
        std::cout << "=== Enjoy your movie! ===\n\n";
    }

    void endMovie()
    {
        std::cout << "\n=== Shutting down theater ===\n";
        popcorn_->off();
        lights_->on();
        screen_->up();
        projector_->off();
        amp_->off();
        dvd_->stop();
        dvd_->eject();
        dvd_->off();
        std::cout << "=== Theater shutdown complete ===\n\n";
    }

    void listenToMusic(const std::string &album)
    {
        std::cout << "\n=== Setting up music mode ===\n";
        lights_->dim(30);
        amp_->on();
        amp_->setVolume(40);
        // Could integrate music player here
        std::cout << "=== Playing: " << album << " ===\n\n";
    }
};

// ============================================================================
// EXAMPLE 2: Computer startup facade
// ============================================================================

class CPU
{
public:
    void freeze() { std::cout << "[CPU] Freezing\n"; }
    void jump(long position) { std::cout << "[CPU] Jumping to position " << position << "\n"; }
    void execute() { std::cout << "[CPU] Executing\n"; }
};

class Memory
{
public:
    void load(long position, const std::string &data)
    {
        std::cout << "[Memory] Loading at " << position << ": " << data << "\n";
    }
};

class HardDrive
{
public:
    std::string read(long lba, int size)
    {
        std::cout << "[HardDrive] Reading " << size << " bytes from LBA " << lba << "\n";
        return "boot_sector_data";
    }
};

// Facade for complex boot process
class ComputerFacade
{
private:
    CPU cpu_;
    Memory memory_;
    HardDrive hardDrive_;

    static constexpr long BOOT_ADDRESS = 0x00;
    static constexpr long BOOT_SECTOR = 0x00;
    static constexpr int SECTOR_SIZE = 512;

public:
    void start()
    {
        std::cout << "\n=== Starting computer ===\n";
        cpu_.freeze();
        std::string bootSector = hardDrive_.read(BOOT_SECTOR, SECTOR_SIZE);
        memory_.load(BOOT_ADDRESS, bootSector);
        cpu_.jump(BOOT_ADDRESS);
        cpu_.execute();
        std::cout << "=== System ready ===\n\n";
    }
};

// ============================================================================
// EXAMPLE 3: Order processing facade
// ============================================================================

class Inventory
{
public:
    bool checkAvailability(const std::string &productId)
    {
        std::cout << "[Inventory] Checking availability for " << productId << "\n";
        return true; // Simplified
    }

    void reserve(const std::string &productId)
    {
        std::cout << "[Inventory] Reserving " << productId << "\n";
    }
};

class Payment
{
public:
    bool processPayment(const std::string &cardNumber, double amount)
    {
        std::cout << "[Payment] Processing $" << amount
                  << " on card ending " << cardNumber.substr(cardNumber.length() - 4) << "\n";
        return true;
    }
};

class Shipping
{
public:
    void scheduleDelivery(const std::string &address)
    {
        std::cout << "[Shipping] Scheduling delivery to " << address << "\n";
    }
};

class Notification
{
public:
    void sendConfirmation(const std::string &email, const std::string &orderId)
    {
        std::cout << "[Notification] Sending confirmation email to " << email
                  << " for order " << orderId << "\n";
    }
};

class OrderFacade
{
private:
    Inventory inventory_;
    Payment payment_;
    Shipping shipping_;
    Notification notification_;

public:
    bool placeOrder(const std::string &productId, const std::string &cardNumber,
                    const std::string &address, const std::string &email)
    {
        std::cout << "\n=== Processing order ===\n";

        if (!inventory_.checkAvailability(productId))
        {
            std::cout << "Order failed: Product not available\n";
            return false;
        }

        inventory_.reserve(productId);

        if (!payment_.processPayment(cardNumber, 99.99))
        {
            std::cout << "Order failed: Payment declined\n";
            return false;
        }

        shipping_.scheduleDelivery(address);
        notification_.sendConfirmation(email, "ORD123456");

        std::cout << "=== Order completed successfully! ===\n\n";
        return true;
    }
};

// ============================================================================
// EXAMPLE 4: API client facade
// ============================================================================

class HTTPClient
{
public:
    void connect(const std::string &url)
    {
        std::cout << "[HTTP] Connecting to " << url << "\n";
    }
    void sendRequest(const std::string &method, const std::string &data)
    {
        std::cout << "[HTTP] Sending " << method << " request\n";
    }
    std::string getResponse()
    {
        return "{\"status\": \"success\"}";
    }
    void disconnect()
    {
        std::cout << "[HTTP] Disconnecting\n";
    }
};

class JSONParser
{
public:
    std::string parse(const std::string &json)
    {
        std::cout << "[JSON] Parsing response\n";
        return "parsed_data";
    }
    std::string serialize(const std::string &data)
    {
        std::cout << "[JSON] Serializing data\n";
        return "{\"data\": \"" + data + "\"}";
    }
};

class Authentication
{
public:
    std::string getToken()
    {
        std::cout << "[Auth] Getting auth token\n";
        return "auth_token_xyz";
    }
};

// Facade for API calls
class APIClientFacade
{
private:
    HTTPClient http_;
    JSONParser json_;
    Authentication auth_;
    std::string baseUrl_;

public:
    explicit APIClientFacade(const std::string &baseUrl) : baseUrl_(baseUrl) {}

    std::string get(const std::string &endpoint)
    {
        std::cout << "\n=== Making API GET request ===\n";
        std::string token = auth_.getToken();
        http_.connect(baseUrl_ + endpoint);
        http_.sendRequest("GET", "");
        std::string response = http_.getResponse();
        http_.disconnect();
        return json_.parse(response);
    }

    std::string post(const std::string &endpoint, const std::string &data)
    {
        std::cout << "\n=== Making API POST request ===\n";
        std::string token = auth_.getToken();
        std::string jsonData = json_.serialize(data);
        http_.connect(baseUrl_ + endpoint);
        http_.sendRequest("POST", jsonData);
        std::string response = http_.getResponse();
        http_.disconnect();
        return json_.parse(response);
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== FACADE PATTERN DEMO ===\n";

    // Home theater
    std::cout << "\n--- Home Theater Facade ---\n";
    {
        HomeTheaterFacade theater;
        theater.watchMovie("Inception");
        // ... movie plays ...
        theater.endMovie();
    }

    // Computer boot
    std::cout << "\n--- Computer Boot Facade ---\n";
    {
        ComputerFacade computer;
        computer.start();
    }

    // Order processing
    std::cout << "\n--- Order Processing Facade ---\n";
    {
        OrderFacade orderSystem;
        orderSystem.placeOrder("PROD-001", "4111111111111111",
                               "123 Main St", "user@example.com");
    }

    // API client
    std::cout << "\n--- API Client Facade ---\n";
    {
        APIClientFacade api("https://api.example.com");
        api.get("/users");
        api.post("/users", "new_user_data");
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Simplifies complex subsystem with unified interface\n";
    std::cout << "2. Reduces dependencies between client and subsystem\n";
    std::cout << "3. Doesn't prevent direct subsystem access (if needed)\n";
    std::cout << "4. Promotes weak coupling and subsystem independence\n";
    std::cout << "5. Easy to use API for common use cases\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Simplify complex subsystem with many interdependent classes
 * - Provide simple interface to complex library/framework
 * - Layer your system (facade for each subsystem)
 * - Reduce coupling between client and implementation classes
 * - Define entry point to each subsystem level
 *
 * BENEFITS:
 * - Shields clients from subsystem complexity
 * - Promotes weak coupling between subsystem and clients
 * - Easier to use, understand, test subsystem
 * - Flexibility: clients can use subsystem classes directly if needed
 * - Layered architecture support
 *
 * DRAWBACKS:
 * - Facade can become a god object coupled to all subsystem classes
 * - May not provide all functionality of subsystem
 * - Additional layer of indirection
 *
 * DESIGN CONSIDERATIONS:
 * - Make facade a lightweight coordinator, not a feature repository
 * - Consider multiple facades for different client needs
 * - Facade should NOT prevent direct subsystem access
 * - Abstract Facade: Interface for multiple implementations
 * - Don't make subsystem classes inaccessible through facade
 *
 * COMPARISON:
 * - Facade: Simplifies interface, doesn't add functionality
 * - Adapter: Changes interface to make incompatible things work
 * - Mediator: Coordinates communication between colleagues (peer-to-peer)
 * - Facade: Unidirectional, client → subsystem only
 */
