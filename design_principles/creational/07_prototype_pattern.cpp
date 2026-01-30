/**
 * PROTOTYPE PATTERN
 *
 * Intent: Specify the kinds of objects to create using a prototypical instance,
 *         and create new objects by copying this prototype.
 *
 * Key Characteristics:
 * - Cloning instead of creating from scratch
 * - Deep copy vs shallow copy
 * - Clone method returns copy of itself
 * - Prototype registry for managing prototypes
 * - Avoids expensive initialization
 *
 * When to Use:
 * - Object creation is expensive (DB queries, network calls, complex init)
 * - Classes to instantiate are specified at runtime
 * - Avoid parallel class hierarchies of factories
 * - Instances have only few state combinations
 * - Want to avoid subclasses of creator (Factory pattern alternative)
 *
 * Common Use Cases:
 * - Game objects (characters, enemies with similar stats)
 * - Document templates
 * - Configuration objects
 * - UI widget templates
 * - Test fixtures
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

// ============================================================================
// PROBLEM: Expensive object creation
// ============================================================================

namespace problem_expensive_creation
{
    class DatabaseRecord
    {
    private:
        string data;
        vector<string> relatedRecords;

    public:
        DatabaseRecord(const string &id)
        {
            cout << "⏳ Expensive operation: Querying database for ID: " << id << "...\n";
            cout << "⏳ Loading related records...\n";
            cout << "⏳ Processing complex calculations...\n";

            // Simulate expensive operations
            data = "Data for " + id;
            relatedRecords = {"Related1", "Related2", "Related3"};

            cout << "✓ Record created (took significant time)\n\n";
        }

        void display() const
        {
            cout << "Data: " << data << "\n";
            cout << "Related: " << relatedRecords.size() << " records\n";
        }
    };

    void demonstrate()
    {
        cout << "=== PROBLEM: Expensive Object Creation ===\n";

        cout << "Creating first record:\n";
        DatabaseRecord rec1("USER_001");
        rec1.display();

        cout << "\nCreating similar record (still expensive!):\n";
        DatabaseRecord rec2("USER_002"); // Same expensive process again!
        rec2.display();

        cout << "\nProblem: Each object creation repeats expensive operations!\n";
        cout << "Even when objects are similar, we pay full cost.\n";
    }
}

// ============================================================================
// SOLUTION: Prototype Pattern (Basic Clone)
// ============================================================================

namespace prototype_basic
{
    // Prototype interface
    class Prototype
    {
    public:
        virtual ~Prototype() = default;
        virtual unique_ptr<Prototype> clone() const = 0;
        virtual void display() const = 0;
    };

    // Concrete prototype
    class Document : public Prototype
    {
    private:
        string title;
        string content;
        vector<string> tags;

    public:
        Document(const string &t, const string &c, const vector<string> &tgs)
            : title(t), content(c), tags(tgs)
        {
            cout << "Creating new document (expensive operation)\n";
        }

        // Copy constructor (used by clone)
        Document(const Document &other)
            : title(other.title), content(other.content), tags(other.tags)
        {
            cout << "Cloning document (fast operation)\n";
        }

        unique_ptr<Prototype> clone() const override
        {
            return make_unique<Document>(*this);
        }

        void setTitle(const string &t) { title = t; }
        void setContent(const string &c) { content = c; }

        void display() const override
        {
            cout << "Document: " << title << "\n";
            cout << "Content: " << content << "\n";
            cout << "Tags: ";
            for (const auto &tag : tags)
            {
                cout << tag << " ";
            }
            cout << "\n";
        }
    };

    void demonstrate()
    {
        cout << "\n=== SOLUTION: Basic Prototype Pattern ===\n";

        // Create original (expensive)
        cout << "Creating original template:\n";
        Document original("Template", "This is a template document", {"draft", "template"});
        original.display();

        // Clone (cheap)
        cout << "\nCloning from template:\n";
        auto doc1 = dynamic_cast<Document *>(original.clone().get());
        if (doc1)
        {
            doc1->setTitle("Document 1");
            doc1->setContent("Content for document 1");
            doc1->display();
        }

        cout << "\nCloning again:\n";
        auto doc2Ptr = original.clone();
        auto doc2 = dynamic_cast<Document *>(doc2Ptr.get());
        if (doc2)
        {
            doc2->setTitle("Document 2");
            doc2->setContent("Content for document 2");
            doc2->display();
        }

        cout << "\nBenefit: Fast cloning instead of expensive creation!\n";
    }
}

// ============================================================================
// REAL-WORLD EXAMPLE: Game Character System
// ============================================================================

namespace game_characters
{
    // Forward declaration
    class Weapon;
    class Armor;

    class Weapon
    {
    public:
        string name;
        int damage;

        Weapon(const string &n, int d) : name(n), damage(d) {}

        Weapon *clone() const
        {
            return new Weapon(*this);
        }

        void display() const
        {
            cout << "    Weapon: " << name << " (Damage: " << damage << ")\n";
        }
    };

    class Armor
    {
    public:
        string name;
        int defense;

        Armor(const string &n, int d) : name(n), defense(d) {}

        Armor *clone() const
        {
            return new Armor(*this);
        }

        void display() const
        {
            cout << "    Armor: " << name << " (Defense: " << defense << ")\n";
        }
    };

    // Character prototype
    class Character
    {
    protected:
        string name;
        int health;
        int mana;
        int strength;
        int intelligence;
        Weapon *weapon;
        Armor *armor;
        vector<string> abilities;

    public:
        Character(const string &n, int hp, int mp, int str, int intel)
            : name(n), health(hp), mana(mp), strength(str), intelligence(intel),
              weapon(nullptr), armor(nullptr) {}

        virtual ~Character()
        {
            delete weapon;
            delete armor;
        }

        // Deep copy clone
        virtual Character *clone() const = 0;

        void setName(const string &n) { name = n; }

        void setWeapon(Weapon *w)
        {
            delete weapon;
            weapon = w;
        }

        void setArmor(Armor *a)
        {
            delete armor;
            armor = a;
        }

        void addAbility(const string &ability)
        {
            abilities.push_back(ability);
        }

        virtual void display() const
        {
            cout << "\n=== Character: " << name << " ===\n";
            cout << "Health: " << health << " | Mana: " << mana << "\n";
            cout << "Strength: " << strength << " | Intelligence: " << intelligence << "\n";
            if (weapon)
                weapon->display();
            if (armor)
                armor->display();
            if (!abilities.empty())
            {
                cout << "  Abilities: ";
                for (const auto &ability : abilities)
                {
                    cout << ability << " ";
                }
                cout << "\n";
            }
        }

    protected:
        // Helper for deep copy
        void copyFrom(const Character &other)
        {
            name = other.name;
            health = other.health;
            mana = other.mana;
            strength = other.strength;
            intelligence = other.intelligence;
            weapon = other.weapon ? other.weapon->clone() : nullptr;
            armor = other.armor ? other.armor->clone() : nullptr;
            abilities = other.abilities;
        }
    };

    // Concrete prototypes
    class Warrior : public Character
    {
    public:
        Warrior() : Character("Warrior", 150, 50, 80, 30)
        {
            setWeapon(new Weapon("Iron Sword", 25));
            setArmor(new Armor("Steel Armor", 40));
            addAbility("Power Strike");
            addAbility("Shield Bash");
        }

        Character *clone() const override
        {
            Warrior *warrior = new Warrior();
            warrior->copyFrom(*this);
            return warrior;
        }
    };

    class Mage : public Character
    {
    public:
        Mage() : Character("Mage", 80, 200, 20, 90)
        {
            setWeapon(new Weapon("Magic Staff", 15));
            setArmor(new Armor("Cloth Robe", 10));
            addAbility("Fireball");
            addAbility("Ice Blast");
            addAbility("Teleport");
        }

        Character *clone() const override
        {
            Mage *mage = new Mage();
            mage->copyFrom(*this);
            return mage;
        }
    };

    class Archer : public Character
    {
    public:
        Archer() : Character("Archer", 100, 80, 60, 50)
        {
            setWeapon(new Weapon("Longbow", 30));
            setArmor(new Armor("Leather Armor", 20));
            addAbility("Multi-shot");
            addAbility("Snipe");
        }

        Character *clone() const override
        {
            Archer *archer = new Archer();
            archer->copyFrom(*this);
            return archer;
        }
    };

    // Prototype Registry
    class CharacterRegistry
    {
    private:
        map<string, Character *> prototypes;

        CharacterRegistry()
        {
            // Register default prototypes
            prototypes["warrior"] = new Warrior();
            prototypes["mage"] = new Mage();
            prototypes["archer"] = new Archer();
        }

        ~CharacterRegistry()
        {
            for (auto &[key, proto] : prototypes)
            {
                delete proto;
            }
        }

        CharacterRegistry(const CharacterRegistry &) = delete;
        CharacterRegistry &operator=(const CharacterRegistry &) = delete;

    public:
        static CharacterRegistry &getInstance()
        {
            static CharacterRegistry instance;
            return instance;
        }

        Character *createCharacter(const string &type)
        {
            auto it = prototypes.find(type);
            if (it != prototypes.end())
            {
                return it->second->clone();
            }
            return nullptr;
        }

        void registerPrototype(const string &type, Character *prototype)
        {
            if (prototypes.find(type) != prototypes.end())
            {
                delete prototypes[type];
            }
            prototypes[type] = prototype;
        }
    };

    void demonstrate()
    {
        cout << "\n=== GAME CHARACTER SYSTEM (Prototype Registry) ===\n";

        CharacterRegistry &registry = CharacterRegistry::getInstance();

        // Create characters by cloning prototypes
        cout << "Creating warrior team:\n";
        Character *warrior1 = registry.createCharacter("warrior");
        warrior1->setName("Thorin");
        warrior1->display();

        Character *warrior2 = registry.createCharacter("warrior");
        warrior2->setName("Gimli");
        warrior2->display();

        cout << "\nCreating mage:\n";
        Character *mage1 = registry.createCharacter("mage");
        mage1->setName("Gandalf");
        mage1->display();

        cout << "\nCreating archer:\n";
        Character *archer1 = registry.createCharacter("archer");
        archer1->setName("Legolas");
        archer1->display();

        // Cleanup
        delete warrior1;
        delete warrior2;
        delete mage1;
        delete archer1;

        cout << "\nBenefits:\n";
        cout << "✓ Fast character creation (no initialization overhead)\n";
        cout << "✓ Consistent base stats for each class\n";
        cout << "✓ Easy customization after cloning\n";
        cout << "✓ Centralized prototype management\n";
    }
}

// ============================================================================
// DEEP COPY vs SHALLOW COPY Example
// ============================================================================

namespace copy_demonstration
{
    class Resource
    {
    public:
        string data;
        Resource(const string &d) : data(d)
        {
            cout << "  Resource created: " << data << "\n";
        }
        ~Resource()
        {
            cout << "  Resource destroyed: " << data << "\n";
        }
    };

    class ShallowCopyObject
    {
    public:
        Resource *resource;

        ShallowCopyObject(const string &data)
        {
            resource = new Resource(data);
        }

        // Shallow copy - DANGEROUS!
        ShallowCopyObject(const ShallowCopyObject &other)
        {
            resource = other.resource; // Same pointer!
            cout << "  Shallow copy made (shares resource pointer)\n";
        }

        ~ShallowCopyObject()
        {
            delete resource; // Double deletion problem!
        }
    };

    class DeepCopyObject
    {
    public:
        Resource *resource;

        DeepCopyObject(const string &data)
        {
            resource = new Resource(data);
        }

        // Deep copy - SAFE
        DeepCopyObject(const DeepCopyObject &other)
        {
            resource = new Resource(other.resource->data); // New copy!
            cout << "  Deep copy made (new resource created)\n";
        }

        ~DeepCopyObject()
        {
            delete resource;
        }

        DeepCopyObject *clone() const
        {
            return new DeepCopyObject(*this);
        }
    };

    void demonstrate()
    {
        cout << "\n=== DEEP COPY vs SHALLOW COPY ===\n";

        cout << "\n--- Shallow Copy (Problematic) ---\n";
        // Note: This example shows the problem but is commented out
        // to avoid actual double-delete crash
        // {
        //     ShallowCopyObject obj1("Original");
        //     ShallowCopyObject obj2(obj1);  // Shallow copy
        //     // Problem: obj1 and obj2 share same resource pointer
        //     // When scope ends, double deletion occurs!
        // }

        cout << "Shallow copy shares pointers - leads to:\n";
        cout << "  ❌ Double deletion\n";
        cout << "  ❌ Dangling pointers\n";
        cout << "  ❌ Undefined behavior\n";

        cout << "\n--- Deep Copy (Correct) ---\n";
        {
            DeepCopyObject obj1("Original");
            DeepCopyObject obj2(obj1); // Deep copy
            DeepCopyObject *obj3 = obj1.clone();

            cout << "Deep copy creates new resources\n";
            cout << "  ✓ Independent objects\n";
            cout << "  ✓ Safe deletion\n";
            cout << "  ✓ No shared state\n";

            delete obj3;
        }
        cout << "\n(All objects destroyed safely)\n";
    }
}

// ============================================================================
// COMPARISON AND GUIDELINES
// ============================================================================

void printGuidelines()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "PROTOTYPE PATTERN - KEY POINTS\n";
    cout << string(70, '=') << "\n\n";

    cout << "WHEN TO USE:\n";
    cout << "  ✓ Object creation is expensive\n";
    cout << "  ✓ Need many similar objects\n";
    cout << "  ✓ Classes specified at runtime\n";
    cout << "  ✓ Want to avoid factory hierarchies\n";
    cout << "  ✓ Object has few state variations\n\n";

    cout << "IMPLEMENTATION APPROACHES:\n";
    cout << "  1. Clone method (virtual function)\n";
    cout << "  2. Copy constructor\n";
    cout << "  3. Prototype registry (centralized management)\n";
    cout << "  4. Deep copy for complex objects\n\n";

    cout << "DEEP COPY vs SHALLOW COPY:\n";
    cout << "  Shallow: Copies pointers (shares data) - DANGEROUS\n";
    cout << "  Deep: Copies data itself (independent) - SAFE\n";
    cout << "  Always use deep copy for Prototype pattern!\n\n";

    cout << "BENEFITS:\n";
    cout << "  • Avoids expensive initialization\n";
    cout << "  • Adds/removes products at runtime\n";
    cout << "  • Reduces subclassing\n";
    cout << "  • Configures application dynamically\n\n";

    cout << "DRAWBACKS:\n";
    cout << "  • Complex deep copy implementation\n";
    cout << "  • Circular references challenging\n";
    cout << "  • Clone method must be updated when class changes\n\n";

    cout << "PROTOTYPE vs FACTORY:\n";
    cout << "  Factory: Creates from scratch\n";
    cout << "  Prototype: Creates by cloning\n";
    cout << "  Use Prototype when initialization is expensive\n\n";

    cout << "COMMON USES:\n";
    cout << "  • Game object templates (characters, items)\n";
    cout << "  • Document templates\n";
    cout << "  • Configuration presets\n";
    cout << "  • Test fixtures\n";
    cout << "  • UI widget templates\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "PROTOTYPE PATTERN\n";
    cout << string(70, '=') << "\n";

    problem_expensive_creation::demonstrate();
    prototype_basic::demonstrate();
    game_characters::demonstrate();
    copy_demonstration::demonstrate();

    printGuidelines();

    cout << "\nKEY TAKEAWAY:\n";
    cout << "Prototype creates new objects by cloning existing ones,\n";
    cout << "avoiding expensive initialization and enabling runtime configuration.\n";
    cout << "\nREMEMBER: Always use DEEP COPY for proper cloning!\n";

    return 0;
}
