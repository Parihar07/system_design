/**
 * PROXY PATTERN
 *
 * Intent: Provide a surrogate or placeholder for another object to control access to it.
 * Acts as an interface to something else (real subject).
 *
 * Problem: Need to control access, add functionality, or defer object creation.
 * Solution: Introduce an intermediary object with same interface as real object.
 *
 * Real-world analogy: Credit card is proxy for bank account - controls access
 *
 * SOLID relation:
 * - OCP: Add proxy behavior without modifying real subject
 * - LSP: Proxy substitutes for real subject seamlessly
 * - SRP: Proxy handles access control, real subject handles business logic
 */

#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>

// ============================================================================
// EXAMPLE 1: Image Loading Problem (Virtual Proxy)
// ============================================================================

// PROBLEM: Expensive object creation upfront
// ---
// Your image viewer loads document with 100 high-resolution images:
// - Each image loads from disk: ~500ms
// - Each image loads to memory: ~50MB
// - Total: 100 × 500ms = 50 seconds + 5GB RAM
//
// Without Proxy:
// ✗ Open document → waits 50 SECONDS for ALL images to load
// ✗ Consume 5GB RAM even if user only views first 3 images
// ✗ User left staring at blank screen (terrible UX!)
// ✗ Wasted resources on images user never sees
// ✗ Violates principle of least surprise
//
// Problems:
// ✗ Upfront cost is prohibitive
// ✗ Memory bloat - loading images never used
// ✗ Slow startup - must wait for everything
// ✗ Poor user experience - frozen UI
// ✗ Wasteful - resource allocation on unnecessary objects
//
// SOLUTION: Virtual Proxy - Lazy initialization
// ---
// Proxy appears to be real image but delays creation:
// 1. Create proxy immediately (no loading!)
// 2. Return proxy to client (super fast!)
// 3. When client first accesses image → load only then
// 4. Cache loaded image for subsequent accesses
//
// Now with Proxy:
// ✓ Open document → instant (just create proxies)
// ✓ Memory: only load images actually viewed
// ✓ User sees content immediately (good UX!)
// ✓ Background load images as needed
// ✓ If user never views image #50, never load it!
//
// Example:
// ImageProxy img1("large1.jpg");  // Instant - no load!
// ImageProxy img2("large2.jpg");  // Instant - no load!
// img1.display();                  // NOW loads - takes 500ms
// img1.display();                  // Cached - instant!
//
// Benefits:
// ✓ Dramatic performance improvement
// ✓ Reduced memory - only loaded what's needed
// ✓ Better UX - responsive app
// ✓ Transparent to client - proxy looks like real image
// ✓ Automatic caching
// ✓ Follows principle of deferred execution

class IImage
{
public:
    virtual ~IImage() = default;
    virtual void display() = 0;
    virtual void rotate(int degrees) = 0;
};
class RealImage : public IImage
{
private:
    std::string filename_;

    void loadFromDisk()
    {
        std::cout << "[RealImage] Loading " << filename_ << " from disk...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate slow loading
        std::cout << "[RealImage] Loaded!\n";
    }

public:
    explicit RealImage(const std::string &filename) : filename_(filename)
    {
        loadFromDisk();
    }

    void display() override
    {
        std::cout << "[RealImage] Displaying " << filename_ << "\n";
    }

    void rotate(int degrees) override
    {
        std::cout << "[RealImage] Rotating " << filename_ << " by " << degrees << " degrees\n";
    }
};

// Virtual proxy - delays creation until needed
class ImageProxy : public IImage
{
private:
    std::string filename_;
    std::unique_ptr<RealImage> realImage_;

    void ensureLoaded()
    {
        if (!realImage_)
        {
            std::cout << "[ImageProxy] First access, loading real image...\n";
            realImage_ = std::make_unique<RealImage>(filename_);
        }
    }

public:
    explicit ImageProxy(const std::string &filename) : filename_(filename)
    {
        std::cout << "[ImageProxy] Created proxy (real image not loaded yet)\n";
    }

    void display() override
    {
        ensureLoaded();
        realImage_->display();
    }

    void rotate(int degrees) override
    {
        ensureLoaded();
        realImage_->rotate(degrees);
    }
};

// ============================================================================
// TYPE 2: PROTECTION PROXY (Access control)
// ============================================================================

class IDocument
{
public:
    virtual ~IDocument() = default;
    virtual void read() = 0;
    virtual void write(const std::string &content) = 0;
    virtual void deleteDocument() = 0;
};

class RealDocument : public IDocument
{
private:
    std::string content_;
    std::string name_;

public:
    explicit RealDocument(const std::string &name) : name_(name), content_("") {}

    void read() override
    {
        std::cout << "[RealDocument] Reading: " << name_ << "\n";
        std::cout << "Content: " << content_ << "\n";
    }

    void write(const std::string &content) override
    {
        std::cout << "[RealDocument] Writing to: " << name_ << "\n";
        content_ = content;
    }

    void deleteDocument() override
    {
        std::cout << "[RealDocument] Deleting: " << name_ << "\n";
    }
};

enum class Permission
{
    READ,
    WRITE,
    DELETE
};

class ProtectedDocumentProxy : public IDocument
{
private:
    std::unique_ptr<RealDocument> document_;
    Permission userPermission_;

    bool hasPermission(Permission required) const
    {
        return static_cast<int>(userPermission_) >= static_cast<int>(required);
    }

public:
    ProtectedDocumentProxy(const std::string &name, Permission permission)
        : document_(std::make_unique<RealDocument>(name)),
          userPermission_(permission) {}

    void read() override
    {
        if (hasPermission(Permission::READ))
        {
            document_->read();
        }
        else
        {
            std::cout << "[ProtectedProxy] Access denied: No read permission\n";
        }
    }

    void write(const std::string &content) override
    {
        if (hasPermission(Permission::WRITE))
        {
            document_->write(content);
        }
        else
        {
            std::cout << "[ProtectedProxy] Access denied: No write permission\n";
        }
    }

    void deleteDocument() override
    {
        if (hasPermission(Permission::DELETE))
        {
            document_->deleteDocument();
        }
        else
        {
            std::cout << "[ProtectedProxy] Access denied: No delete permission\n";
        }
    }
};

// ============================================================================
// TYPE 3: CACHING PROXY (Performance optimization)
// ============================================================================

class IWeatherService
{
public:
    virtual ~IWeatherService() = default;
    virtual std::string getWeather(const std::string &city) = 0;
};

class RealWeatherService : public IWeatherService
{
public:
    std::string getWeather(const std::string &city) override
    {
        std::cout << "[RealWeatherService] Fetching weather from API for " << city << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulate network delay
        return "Sunny, 25°C";
    }
};

class CachingWeatherProxy : public IWeatherService
{
private:
    std::unique_ptr<RealWeatherService> service_;
    std::unordered_map<std::string, std::string> cache_;

public:
    CachingWeatherProxy() : service_(std::make_unique<RealWeatherService>()) {}

    std::string getWeather(const std::string &city) override
    {
        auto it = cache_.find(city);
        if (it != cache_.end())
        {
            std::cout << "[CachingProxy] Cache hit for " << city << "\n";
            return it->second;
        }

        std::cout << "[CachingProxy] Cache miss for " << city << ", fetching...\n";
        std::string weather = service_->getWeather(city);
        cache_[city] = weather;
        return weather;
    }
};

// ============================================================================
// TYPE 4: LOGGING PROXY (Audit trail)
// ============================================================================

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual void executeQuery(const std::string &query) = 0;
};

class RealDatabase : public IDatabase
{
public:
    void executeQuery(const std::string &query) override
    {
        std::cout << "[RealDatabase] Executing: " << query << "\n";
    }
};

class LoggingDatabaseProxy : public IDatabase
{
private:
    std::unique_ptr<RealDatabase> database_;
    std::string username_;

    void log(const std::string &action)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "[LoggingProxy] " << std::ctime(&time)
                  << "User: " << username_ << ", Action: " << action << "\n";
    }

public:
    LoggingDatabaseProxy(const std::string &username)
        : database_(std::make_unique<RealDatabase>()), username_(username) {}

    void executeQuery(const std::string &query) override
    {
        log("Query: " + query);
        database_->executeQuery(query);
        log("Query completed");
    }
};

// ============================================================================
// TYPE 5: REMOTE PROXY (Network access)
// ============================================================================

class IRemoteService
{
public:
    virtual ~IRemoteService() = default;
    virtual std::string processRequest(const std::string &data) = 0;
};

// Simulates remote service
class RealRemoteService : public IRemoteService
{
public:
    std::string processRequest(const std::string &data) override
    {
        std::cout << "[RealRemoteService] Processing: " << data << "\n";
        return "Processed: " + data;
    }
};

// Remote proxy handles network communication
class RemoteServiceProxy : public IRemoteService
{
private:
    std::unique_ptr<RealRemoteService> service_;
    std::string serverAddress_;

    void connect()
    {
        std::cout << "[RemoteProxy] Connecting to " << serverAddress_ << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "[RemoteProxy] Connected!\n";
    }

    void disconnect()
    {
        std::cout << "[RemoteProxy] Disconnecting...\n";
    }

    std::string serialize(const std::string &data)
    {
        return "[SERIALIZED:" + data + "]";
    }

    std::string deserialize(const std::string &data)
    {
        return data; // Simplified
    }

public:
    explicit RemoteServiceProxy(const std::string &address)
        : service_(std::make_unique<RealRemoteService>()), serverAddress_(address) {}

    std::string processRequest(const std::string &data) override
    {
        connect();
        std::string serialized = serialize(data);
        std::cout << "[RemoteProxy] Sending: " << serialized << "\n";

        std::string result = service_->processRequest(data);

        std::cout << "[RemoteProxy] Received response\n";
        disconnect();
        return deserialize(result);
    }
};

// ============================================================================
// TYPE 6: SMART REFERENCE PROXY (Reference counting, copy-on-write)
// ============================================================================

class LargeObject
{
private:
    std::string data_;

public:
    explicit LargeObject(const std::string &data) : data_(data)
    {
        std::cout << "[LargeObject] Created with data: " << data << "\n";
    }

    std::string getData() const { return data_; }
    void setData(const std::string &data) { data_ = data; }
};

class SmartReferenceProxy
{
private:
    std::shared_ptr<LargeObject> object_;
    int *referenceCount_;

public:
    explicit SmartReferenceProxy(const std::string &data)
        : object_(std::make_shared<LargeObject>(data)),
          referenceCount_(new int(1))
    {
        std::cout << "[SmartProxy] Created, ref count: " << *referenceCount_ << "\n";
    }

    SmartReferenceProxy(const SmartReferenceProxy &other)
        : object_(other.object_), referenceCount_(other.referenceCount_)
    {
        (*referenceCount_)++;
        std::cout << "[SmartProxy] Copied, ref count: " << *referenceCount_ << "\n";
    }

    ~SmartReferenceProxy()
    {
        (*referenceCount_)--;
        std::cout << "[SmartProxy] Destroyed, ref count: " << *referenceCount_ << "\n";
        if (*referenceCount_ == 0)
        {
            delete referenceCount_;
            std::cout << "[SmartProxy] Last reference, cleaning up\n";
        }
    }

    std::string getData() const
    {
        return object_->getData();
    }
};

// ============================================================================
// Demonstration
// ============================================================================

void demonstrateVirtualProxy()
{
    std::cout << "\n--- Virtual Proxy (Lazy Loading) ---\n";
    std::cout << "Creating image proxies...\n";
    ImageProxy img1("large_image1.jpg");
    ImageProxy img2("large_image2.jpg");

    std::cout << "\nAccessing first image:\n";
    img1.display();

    std::cout << "\nAccessing first image again (already loaded):\n";
    img1.display();

    std::cout << "\nSecond image never accessed (never loaded!)\n";
}

void demonstrateProtectionProxy()
{
    std::cout << "\n--- Protection Proxy (Access Control) ---\n";

    std::cout << "\nUser with READ permission:\n";
    ProtectedDocumentProxy doc1("sensitive.txt", Permission::READ);
    doc1.read();
    doc1.write("Attempt to modify");
    doc1.deleteDocument();

    std::cout << "\nUser with DELETE permission (has all permissions):\n";
    ProtectedDocumentProxy doc2("admin.txt", Permission::DELETE);
    doc2.read();
    doc2.write("Admin can modify");
    doc2.deleteDocument();
}

void demonstrateCachingProxy()
{
    std::cout << "\n--- Caching Proxy (Performance) ---\n";
    CachingWeatherProxy weather;

    std::cout << "First request for London:\n";
    std::cout << "Weather: " << weather.getWeather("London") << "\n\n";

    std::cout << "Second request for London (cached):\n";
    std::cout << "Weather: " << weather.getWeather("London") << "\n\n";

    std::cout << "Request for Paris:\n";
    std::cout << "Weather: " << weather.getWeather("Paris") << "\n";
}

void demonstrateLoggingProxy()
{
    std::cout << "\n--- Logging Proxy (Audit Trail) ---\n";
    LoggingDatabaseProxy db("admin_user");
    db.executeQuery("SELECT * FROM users");
    db.executeQuery("DELETE FROM sessions WHERE expired = true");
}

void demonstrateRemoteProxy()
{
    std::cout << "\n--- Remote Proxy (Network Access) ---\n";
    RemoteServiceProxy service("https://api.example.com");
    std::string result = service.processRequest("user_data");
    std::cout << "Result: " << result << "\n";
}

void demonstrateSmartReference()
{
    std::cout << "\n--- Smart Reference Proxy ---\n";
    {
        SmartReferenceProxy proxy1("Large data block");
        {
            SmartReferenceProxy proxy2 = proxy1;
            SmartReferenceProxy proxy3 = proxy1;
            std::cout << "Multiple references to same object\n";
        }
        std::cout << "Inner scope ended\n";
    }
    std::cout << "All references gone\n";
}

int main()
{
    std::cout << "=== PROXY PATTERN DEMO ===\n";

    demonstrateVirtualProxy();
    demonstrateProtectionProxy();
    demonstrateCachingProxy();
    demonstrateLoggingProxy();
    demonstrateRemoteProxy();
    demonstrateSmartReference();

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Same interface as real subject (transparent to client)\n";
    std::cout << "2. Controls access and adds behavior without modifying subject\n";
    std::cout << "3. Multiple types: Virtual, Protection, Caching, Logging, Remote, Smart\n";
    std::cout << "4. Proxy delegates to real subject after preprocessing\n";
    std::cout << "5. Can delay expensive object creation (virtual)\n";
    std::cout << "6. Can add security, logging, caching transparently\n";

    return 0;
}

/**
 * PROXY TYPES:
 *
 * 1. Virtual Proxy: Lazy initialization, defer expensive object creation
 * 2. Protection Proxy: Access control based on permissions/credentials
 * 3. Remote Proxy: Local representative for remote object (network)
 * 4. Caching Proxy: Cache results to avoid expensive operations
 * 5. Logging Proxy: Add logging/auditing without changing subject
 * 6. Smart Reference: Reference counting, copy-on-write, auto cleanup
 * 7. Firewall Proxy: Network-level access control
 * 8. Synchronization Proxy: Thread-safe access to subject
 *
 * WHEN TO USE:
 * - Need more sophisticated reference than simple pointer
 * - Want to defer expensive object creation (virtual proxy)
 * - Need access control (protection proxy)
 * - Local representative of remote object (remote proxy)
 * - Add functionality without modifying subject
 *
 * BENEFITS:
 * - OCP: Add behavior without modifying subject
 * - Controls object lifecycle and access
 * - Transparent to client (same interface)
 * - Performance optimization (lazy loading, caching)
 * - Security (access control)
 * - Separation of concerns (proxy handles cross-cutting)
 *
 * DRAWBACKS:
 * - Additional indirection (slight performance cost)
 * - Response delay (especially remote proxy)
 * - Complexity increase
 *
 * COMPARISON:
 * - Proxy: Same interface, controls access, doesn't add behavior
 * - Decorator: Same interface, adds behavior/responsibility
 * - Adapter: Different interface, makes incompatible things work
 * - Facade: Simplified interface, multiple subsystem classes
 */
