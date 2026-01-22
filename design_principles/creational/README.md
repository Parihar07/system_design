# Creational Design Patterns

Patterns focused on object creation mechanisms, providing flexibility in what gets created, who creates it, how it gets created, and when.

## Patterns in This Folder

### Singleton
- **Files:** `01_singleton_problem.cpp`, `02_singleton_solution.cpp`
- **Intent:** Ensure a class has only one instance with global access
- **Use Cases:** Logger, Config Manager, Database Connection Pool
- **Key Concept:** Private constructor + static instance

### Factory Method
- **Files:** `03_factory_pattern.cpp`
- **Intent:** Define interface for creating objects, let subclasses decide which class to instantiate
- **Use Cases:** UI frameworks (cross-platform widgets), document editors
- **Key Concept:** Virtual constructor, delegates creation to subclasses

### Abstract Factory
- **Status:** Coming soon
- **Intent:** Create families of related objects without specifying concrete classes
- **Use Cases:** Cross-platform UI toolkits, database drivers

### Builder
- **Status:** Coming soon
- **Intent:** Construct complex objects step by step
- **Use Cases:** HTTP request builders, document builders

### Prototype
- **Status:** Coming soon
- **Intent:** Clone objects without coupling to their concrete classes
- **Use Cases:** Object copying, registry of prototypical instances

## Build Examples

```bash
# Singleton problem demonstration
make FILE=01_singleton_problem.cpp run

# Singleton solution
make FILE=02_singleton_solution.cpp run

# Factory Method
make FILE=03_factory_pattern.cpp run
```

## Key Takeaways

**When to use Creational Patterns:**
- Object creation is complex
- Need to hide creation logic
- Want to decouple client from concrete classes
- Need to manage object lifecycle
- Require flexibility in what gets created

**Common Interview Questions:**
- Why is Singleton considered an anti-pattern?
- Difference between Factory Method and Abstract Factory?
- When would you use Builder over constructor?
- How to make Singleton thread-safe?
- Prototype vs Copy Constructor?
