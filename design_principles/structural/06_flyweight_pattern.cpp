/**
 * FLYWEIGHT PATTERN
 *
 * Intent: Use sharing to support large numbers of fine-grained objects efficiently.
 * Minimizes memory use by sharing common data (intrinsic state) among similar objects.
 *
 * Problem: Creating many similar objects consumes too much memory.
 * Solution: Share immutable common state; store unique state externally.
 *
 * Real-world analogy: Library books - one physical copy shared by many borrowers
 *
 * SOLID relation:
 * - SRP: Separate intrinsic (shared) from extrinsic (unique) state
 * - Memory efficiency through object pooling and sharing
 */

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

// ============================================================================
// EXAMPLE 1: Game Particles Memory Problem
// ============================================================================

// PROBLEM: Memory explosion with many similar objects
// ---
// Game has thousands of particles (bullets, explosions, rain):
// - 50,000 bullets with sprite "bullet.png", color RED
// - 30,000 explosions with sprite "explosion.png", color YELLOW
// - 100,000 rain drops with sprite "rain.png", color BLUE
//
// Each particle WITHOUT Flyweight stores:
// struct Particle {
//     int x, y;                    // 8 bytes
//     int speed;                   // 4 bytes
//     std::string sprite;          // ~32 bytes (same for all bullets!)
//     std::string name;            // ~32 bytes (replicated!)
//     int color;                   // 4 bytes
// };  // ~80 bytes per particle
//
// Total memory: 180,000 particles × 80 bytes = 14.4 MB!
// But wait: 50,000 bullets store "bullet.png" 50,000 times!
// We're storing "bullet.png" string 50,000 times = massive waste!
//
// Problems:
// ✗ Memory explosion - 14.4 MB for what could be a few KB
// ✗ Cache misses - poor CPU cache utilization with big objects
// ✗ GC pressure - so many allocations stress memory manager
// ✗ Performance suffers - memory bandwidth becomes bottleneck
// ✗ 99% of particle data is DUPLICATED across particles of same type!
//
// SOLUTION: Flyweight Pattern - Share intrinsic state
// ---
// Key insight: Separate state into two types:
// 1. INTRINSIC (shared, immutable): sprite name, color
//    Store ONCE and share reference among all bullet particles
// 2. EXTRINSIC (unique, mutable): position, speed
//    Store per particle
//
// Now:
// - ParticleType: "bullet", sprite, color (SHARED by all bullets)
// - Particle: x, y, speed, pointer to ParticleType (UNIQUE per particle)
//
// Memory calculation:
// - ParticleType × 2 types = 64 bytes total
// - Particle × 180,000 = 180,000 × 16 bytes = 2.88 MB
// - TOTAL: ~2.95 MB (vs 14.4 MB before!) - 80% savings!
//
// Benefits:
// ✓ Massive memory savings - trade memory for CPU lookup
// ✓ Better cache utilization - smaller objects fit in L1/L2 cache
// ✓ Less GC pressure - fewer allocations
// ✓ Better performance - reduced memory bandwidth
// ✓ Scalability - can handle millions of particles
// ✓ Immutable shared objects are thread-safe

/**
 * Intrinsic state: Shared, context-independent, immutable
 * Extrinsic state: Unique per object, context-dependent, passed by client
 */

// ============================================================================
// EXAMPLE 1: Game particles (bullets, trees, etc.)
// ============================================================================

// Flyweight: Contains intrinsic state only
class ParticleType
{
private:
    std::string name_;
    std::string sprite_;
    int color_;

public:
    ParticleType(const std::string &name, const std::string &sprite, int color)
        : name_(name), sprite_(sprite), color_(color) {}

    void render(int x, int y, int speed) const
    {
        std::cout << "[" << name_ << "] Rendering sprite '" << sprite_
                  << "' (color=" << color_ << ") at (" << x << "," << y
                  << ") speed=" << speed << "\n";
    }

    std::string getName() const { return name_; }
};

// Flyweight factory
class ParticleFactory
{
private:
    std::unordered_map<std::string, std::unique_ptr<ParticleType>> types_;

public:
    const ParticleType *getParticleType(const std::string &name,
                                        const std::string &sprite,
                                        int color)
    {
        auto it = types_.find(name);
        if (it == types_.end())
        {
            std::cout << "[Factory] Creating new ParticleType: " << name << "\n";
            types_[name] = std::make_unique<ParticleType>(name, sprite, color);
        }
        return types_[name].get();
    }

    int getTypeCount() const { return types_.size(); }
};

// Context object: Contains extrinsic state + reference to flyweight
class Particle
{
private:
    int x_, y_;                // Extrinsic: position
    int speed_;                // Extrinsic: velocity
    const ParticleType *type_; // Intrinsic: shared type

public:
    Particle(int x, int y, int speed, const ParticleType *type)
        : x_(x), y_(y), speed_(speed), type_(type) {}

    void move()
    {
        x_ += speed_;
        y_ += speed_;
    }

    void render() const
    {
        type_->render(x_, y_, speed_);
    }
};

// Client managing particles
class ParticleSystem
{
private:
    std::vector<Particle> particles_;
    ParticleFactory factory_;

public:
    void addParticle(int x, int y, int speed, const std::string &typeName,
                     const std::string &sprite, int color)
    {
        const ParticleType *type = factory_.getParticleType(typeName, sprite, color);
        particles_.emplace_back(x, y, speed, type);
    }

    void render() const
    {
        std::cout << "\nRendering " << particles_.size() << " particles "
                  << "(using " << factory_.getTypeCount() << " shared types):\n";
        for (const auto &particle : particles_)
        {
            particle.render();
        }
    }
};

// ============================================================================
// EXAMPLE 2: Text editor (character formatting)
// ============================================================================

// Flyweight: Character format
class CharacterFormat
{
private:
    std::string font_;
    int size_;
    bool bold_;
    bool italic_;
    int color_;

public:
    CharacterFormat(const std::string &font, int size, bool bold,
                    bool italic, int color)
        : font_(font), size_(size), bold_(bold), italic_(italic), color_(color) {}

    void render(char c, int position) const
    {
        std::cout << "Char '" << c << "' at pos " << position
                  << " [" << font_ << ", " << size_ << "pt"
                  << (bold_ ? ", bold" : "")
                  << (italic_ ? ", italic" : "")
                  << ", color=" << color_ << "]\n";
    }

    std::string getKey() const
    {
        return font_ + "_" + std::to_string(size_) + "_" + std::to_string(bold_) + "_" + std::to_string(italic_) + "_" + std::to_string(color_);
    }
};

// Flyweight factory
class FormatFactory
{
private:
    std::unordered_map<std::string, std::shared_ptr<CharacterFormat>> formats_;

public:
    std::shared_ptr<CharacterFormat> getFormat(const std::string &font, int size,
                                               bool bold, bool italic, int color)
    {
        CharacterFormat temp(font, size, bold, italic, color);
        std::string key = temp.getKey();

        auto it = formats_.find(key);
        if (it == formats_.end())
        {
            std::cout << "[FormatFactory] Creating new format: " << key << "\n";
            formats_[key] = std::make_shared<CharacterFormat>(font, size, bold, italic, color);
        }
        return formats_[key];
    }

    int getFormatCount() const { return formats_.size(); }
};

// Context: Character with position and format reference
struct FormattedChar
{
    char character;
    int position;
    std::shared_ptr<CharacterFormat> format;

    void render() const
    {
        format->render(character, position);
    }
};

// Document managing characters
class Document
{
private:
    std::vector<FormattedChar> characters_;
    FormatFactory factory_;

public:
    void addCharacter(char c, int pos, const std::string &font, int size,
                      bool bold, bool italic, int color)
    {
        auto format = factory_.getFormat(font, size, bold, italic, color);
        characters_.push_back({c, pos, format});
    }

    void render() const
    {
        std::cout << "\nDocument with " << characters_.size() << " characters "
                  << "(using " << factory_.getFormatCount() << " shared formats):\n";
        for (const auto &fc : characters_)
        {
            fc.render();
        }
    }
};

// ============================================================================
// EXAMPLE 3: Chess game (reusing piece objects)
// ============================================================================

// Flyweight: Chess piece type
class ChessPieceType
{
private:
    std::string name_;
    std::string sprite_;

public:
    ChessPieceType(const std::string &name, const std::string &sprite)
        : name_(name), sprite_(sprite) {}

    void render(int row, int col, const std::string &color) const
    {
        std::cout << color << " " << name_ << " [" << sprite_
                  << "] at (" << row << "," << col << ")\n";
    }
};

// Flyweight factory
class ChessPieceFactory
{
private:
    std::unordered_map<std::string, std::unique_ptr<ChessPieceType>> types_;

public:
    const ChessPieceType *getPieceType(const std::string &name,
                                       const std::string &sprite)
    {
        auto it = types_.find(name);
        if (it == types_.end())
        {
            types_[name] = std::make_unique<ChessPieceType>(name, sprite);
        }
        return types_[name].get();
    }
};

// Context: Piece position and color
struct ChessPiece
{
    int row, col;
    std::string color;
    const ChessPieceType *type;

    void render() const
    {
        type->render(row, col, color);
    }
};

class ChessBoard
{
private:
    std::vector<ChessPiece> pieces_;
    ChessPieceFactory factory_;

public:
    void placePiece(int row, int col, const std::string &color,
                    const std::string &name, const std::string &sprite)
    {
        const ChessPieceType *type = factory_.getPieceType(name, sprite);
        pieces_.push_back({row, col, color, type});
    }

    void render() const
    {
        std::cout << "\nChess board with " << pieces_.size() << " pieces:\n";
        for (const auto &piece : pieces_)
        {
            piece.render();
        }
    }
};

// ============================================================================
// EXAMPLE 4: String pool (like Java string interning)
// ============================================================================

class StringPool
{
private:
    std::unordered_map<std::string, std::shared_ptr<std::string>> pool_;

public:
    std::shared_ptr<std::string> intern(const std::string &str)
    {
        auto it = pool_.find(str);
        if (it == pool_.end())
        {
            std::cout << "[StringPool] Creating new string: \"" << str << "\"\n";
            pool_[str] = std::make_shared<std::string>(str);
        }
        else
        {
            std::cout << "[StringPool] Reusing existing string: \"" << str << "\"\n";
        }
        return pool_[str];
    }

    size_t size() const { return pool_.size(); }

    void stats() const
    {
        std::cout << "\n[StringPool] Total unique strings: " << pool_.size() << "\n";
        size_t totalMemory = 0;
        for (const auto &[key, value] : pool_)
        {
            totalMemory += value->capacity();
        }
        std::cout << "[StringPool] Approximate memory: " << totalMemory << " bytes\n";
    }
};

// ============================================================================
// Demonstration
// ============================================================================

void demonstrateParticles()
{
    std::cout << "\n--- Game Particles Example ---\n";
    ParticleSystem particles;

    // Create many bullets (same type)
    for (int i = 0; i < 5; ++i)
    {
        particles.addParticle(i * 10, i * 10, 5, "Bullet", "bullet.png", 0xFF0000);
    }

    // Create some missiles (different type)
    for (int i = 0; i < 3; ++i)
    {
        particles.addParticle(i * 15, i * 15, 3, "Missile", "missile.png", 0x00FF00);
    }

    particles.render();
    std::cout << "\nMemory saved: Instead of 8 complete objects, "
              << "we have 2 shared types + 8 lightweight contexts\n";
}

void demonstrateTextEditor()
{
    std::cout << "\n--- Text Editor Example ---\n";
    Document doc;

    std::string text = "Hello World";
    for (size_t i = 0; i < text.length(); ++i)
    {
        // Most characters share same format
        bool isBold = (text[i] == 'H' || text[i] == 'W');
        doc.addCharacter(text[i], i, "Arial", 12, isBold, false, 0x000000);
    }

    doc.render();
    std::cout << "\nMemory saved: 11 characters share ~2 format objects\n";
}

void demonstrateChess()
{
    std::cout << "\n--- Chess Board Example ---\n";
    ChessBoard board;

    // Setup some pieces
    board.placePiece(0, 0, "White", "Rook", "♜");
    board.placePiece(0, 7, "White", "Rook", "♜");
    board.placePiece(7, 0, "Black", "Rook", "♜");
    board.placePiece(7, 7, "Black", "Rook", "♜");
    board.placePiece(0, 4, "White", "King", "♚");
    board.placePiece(7, 4, "Black", "King", "♚");

    board.render();
    std::cout << "\nMemory saved: 6 pieces share 2 types (Rook, King)\n";
}

void demonstrateStringPool()
{
    std::cout << "\n--- String Pool Example ---\n";
    StringPool pool;

    auto s1 = pool.intern("Hello");
    auto s2 = pool.intern("World");
    auto s3 = pool.intern("Hello"); // Reuses
    auto s4 = pool.intern("Hello"); // Reuses
    auto s5 = pool.intern("World"); // Reuses

    std::cout << "\ns1 == s3? " << (s1 == s3 ? "YES (same object)" : "NO") << "\n";
    std::cout << "s2 == s5? " << (s2 == s5 ? "YES (same object)" : "NO") << "\n";

    pool.stats();
}

int main()
{
    std::cout << "=== FLYWEIGHT PATTERN DEMO ===\n";

    demonstrateParticles();
    demonstrateTextEditor();
    demonstrateChess();
    demonstrateStringPool();

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Share intrinsic state (immutable, context-independent)\n";
    std::cout << "2. Store extrinsic state separately (mutable, context-specific)\n";
    std::cout << "3. Factory manages shared objects (object pool)\n";
    std::cout << "4. Massive memory savings when many similar objects\n";
    std::cout << "5. Trade CPU (lookup) for memory (sharing)\n";
    std::cout << "6. Objects become immutable sharable values\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Application uses large number of objects
 * - Storage costs high due to object quantity
 * - Most object state can be made extrinsic
 * - Many groups of objects share intrinsic state
 * - Application doesn't depend on object identity
 *
 * BENEFITS:
 * - Significant memory savings
 * - Reduced object count
 * - Centralized state management
 * - Immutability promotes thread safety
 *
 * DRAWBACKS:
 * - Complexity increase (intrinsic vs extrinsic separation)
 * - Runtime cost of computing/passing extrinsic state
 * - Factory lookup overhead
 * - May not help if most state is extrinsic
 *
 * IMPLEMENTATION NOTES:
 * - Flyweight must be immutable (thread-safe sharing)
 * - Factory ensures flyweights are shared properly
 * - Client responsible for computing/storing extrinsic state
 * - Consider weak pointers if flyweights need lifecycle management
 * - May combine with other patterns (Factory, Singleton for factory)
 *
 * COMPARISON:
 * - Flyweight: Share identical objects to save memory
 * - Singleton: Ensure only one instance exists
 * - State: Object changes behavior, not about sharing
 * - Strategy: Interchangeable algorithms, not about memory
 * - Prototype: Clone objects, doesn't share
 * - Object Pool: Reuse for performance, Flyweight for memory
 */
