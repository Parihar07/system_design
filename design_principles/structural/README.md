# Structural Design Patterns

Patterns that deal with object composition, forming larger structures from individual objects while keeping these structures flexible and efficient.

## Patterns in This Folder

### Adapter
- **Status:** Coming soon
- **Intent:** Convert interface of a class into another interface clients expect
- **Use Cases:** Legacy system integration, third-party library wrappers
- **Key Concept:** Interface compatibility layer

### Bridge
- **Status:** Coming soon
- **Intent:** Decouple abstraction from implementation
- **Use Cases:** Device drivers, GUI frameworks with multiple rendering engines
- **Key Concept:** Composition over inheritance

### Composite
- **Status:** Coming soon
- **Intent:** Compose objects into tree structures to represent part-whole hierarchies
- **Use Cases:** File systems, GUI component trees, organization hierarchies
- **Key Concept:** Treat individual objects and compositions uniformly

### Decorator
- **Status:** Coming soon
- **Intent:** Attach additional responsibilities to objects dynamically
- **Use Cases:** I/O streams, UI component enhancement
- **Key Concept:** Wrapper that extends behavior

### Facade
- **Status:** Coming soon
- **Intent:** Provide simplified interface to complex subsystem
- **Use Cases:** Library wrappers, complex API simplification
- **Key Concept:** Unified interface to set of interfaces

### Flyweight
- **Status:** Coming soon
- **Intent:** Share common state to support large numbers of fine-grained objects
- **Use Cases:** Text editors (character objects), game sprites
- **Key Concept:** Intrinsic vs extrinsic state separation

### Proxy
- **Status:** Coming soon
- **Intent:** Provide surrogate or placeholder for another object
- **Use Cases:** Lazy loading, access control, remote proxies
- **Key Concept:** Control access to the real subject

## Build Examples

```bash
# Adapter pattern
make FILE=01_adapter_pattern.cpp run

# Decorator pattern
make FILE=02_decorator_pattern.cpp run
```

## Key Takeaways

**When to use Structural Patterns:**
- Need to compose objects into larger structures
- Want to add functionality without changing existing code
- Need to adapt interfaces
- Want to simplify complex subsystems
- Need to manage memory/resource efficiently

**Common Interview Questions:**
- Adapter vs Bridge vs Facade?
- Decorator vs Inheritance?
- When to use Proxy pattern?
- Composite pattern use cases?
- Flyweight vs Singleton?
