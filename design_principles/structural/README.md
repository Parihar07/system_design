# Structural Design Patterns

Patterns that deal with object composition, forming larger structures from individual objects while keeping these structures flexible and efficient.

## Patterns in This Folder

### 1. Adapter Pattern ([01_adapter_pattern.cpp](01_adapter_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Convert interface of a class into another interface clients expect
- **Use Cases:** Legacy system integration, third-party library wrappers
- **Key Concept:** Interface compatibility layer
- **Examples:** Payment gateway adapter, logger adapter
- **SOLID:** OCP, ISP, DIP

### 2. Bridge Pattern ([02_bridge_pattern.cpp](02_bridge_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Decouple abstraction from implementation
- **Use Cases:** Device drivers, GUI frameworks, cross-platform rendering
- **Key Concept:** Composition over inheritance
- **Examples:** Shape rendering, message senders, remote controls
- **SOLID:** SRP, OCP, DIP

### 3. Composite Pattern ([03_composite_pattern.cpp](03_composite_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Compose objects into tree structures to represent part-whole hierarchies
- **Use Cases:** File systems, GUI component trees, organization hierarchies
- **Key Concept:** Treat individual objects and compositions uniformly
- **Examples:** File system, UI widgets, employee hierarchy
- **SOLID:** OCP, LSP

### 4. Decorator Pattern ([04_decorator_pattern.cpp](04_decorator_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Attach additional responsibilities to objects dynamically
- **Use Cases:** I/O streams, UI component enhancement, notifications
- **Key Concept:** Wrapper that extends behavior
- **Examples:** Coffee shop, data streams (compression/encryption), notifications
- **SOLID:** OCP, SRP, LSP

### 5. Facade Pattern ([05_facade_pattern.cpp](05_facade_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Provide simplified interface to complex subsystem
- **Use Cases:** Library wrappers, complex API simplification
- **Key Concept:** Unified interface to set of interfaces
- **Examples:** Home theater, computer boot, order processing, API client
- **SOLID:** SRP, DIP

### 6. Flyweight Pattern ([06_flyweight_pattern.cpp](06_flyweight_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Share common state to support large numbers of fine-grained objects
- **Use Cases:** Text editors (character objects), game particles, string pooling
- **Key Concept:** Intrinsic vs extrinsic state separation
- **Examples:** Game particles, text formatting, chess pieces, string pool
- **SOLID:** SRP

### 7. Proxy Pattern ([07_proxy_pattern.cpp](07_proxy_pattern.cpp))
- **Status:** ✅ Complete with examples
- **Intent:** Provide surrogate or placeholder for another object
- **Use Cases:** Lazy loading, access control, remote proxies, caching
- **Key Concept:** Control access to the real subject
- **Examples:** Virtual (lazy), Protection (access control), Caching, Logging, Remote, Smart Reference
- **SOLID:** OCP, LSP, SRP

## Build & Run Examples

```bash
# Run individual patterns
make FILE=01_adapter_pattern.cpp run
make FILE=02_bridge_pattern.cpp run
make FILE=03_composite_pattern.cpp run
make FILE=04_decorator_pattern.cpp run
make FILE=05_facade_pattern.cpp run
make FILE=06_flyweight_pattern.cpp run
make FILE=07_proxy_pattern.cpp run

# Build only (no run)
make FILE=01_adapter_pattern.cpp build

# Clean
make clean
```

## Pattern Comparison Matrix

| Pattern | Purpose | Structure | When to Use |
|---------|---------|-----------|-------------|
| **Adapter** | Interface compatibility | Wraps incompatible interface | Legacy integration, third-party libs |
| **Bridge** | Decouple abstraction/impl | Two parallel hierarchies | Avoid class explosion, runtime binding |
| **Composite** | Part-whole hierarchy | Tree structure | Hierarchical data, uniform treatment |
| **Decorator** | Add responsibilities | Nested wrappers | Dynamic behavior extension |
| **Facade** | Simplify subsystem | Unified high-level interface | Complex subsystem, layering |
| **Flyweight** | Share objects | Object pool | Many similar objects, memory critical |
| **Proxy** | Control access | Surrogate/placeholder | Lazy loading, access control, caching |

## Key Interview Questions & Answers

### Q: Adapter vs Bridge vs Facade?
- **Adapter**: Retrofit incompatible interfaces to work together (after-the-fact)
- **Bridge**: Designed upfront to separate abstraction from implementation
- **Facade**: Simplifies complex subsystem, doesn't change interfaces

### Q: Decorator vs Inheritance?
- **Inheritance**: Static, compile-time, all-or-nothing
- **Decorator**: Dynamic, runtime, mix-and-match responsibilities
- Decorator follows OCP better (extend without modifying)

### Q: When to use Proxy pattern?
- **Virtual Proxy**: Expensive object creation (lazy loading)
- **Protection Proxy**: Access control/security
- **Remote Proxy**: Network object representation
- **Caching Proxy**: Performance optimization
- **Logging Proxy**: Audit trail without changing subject

### Q: Composite pattern use cases?
- File systems (files/folders)
- GUI hierarchies (containers/widgets)
- Organization charts (managers/employees)
- Document structures (sections/paragraphs)
- Mathematical expressions (operators/operands)

### Q: Flyweight vs Singleton?
- **Singleton**: One instance globally
- **Flyweight**: Many shared instances (pool of immutable objects)
- **Flyweight**: Focused on memory savings through sharing
- **Singleton**: Focused on controlled access to single instance

### Q: Decorator vs Proxy?
- **Decorator**: Adds new behavior/responsibilities
- **Proxy**: Controls access, doesn't add core functionality
- Both have same interface as subject, but different intent

## SOLID Principles in Structural Patterns

### Single Responsibility Principle (SRP)
- **Bridge**: Abstraction and implementation have separate responsibilities
- **Facade**: Facade coordinates, subsystems handle specifics
- **Proxy**: Proxy handles access control, subject handles business logic

### Open/Closed Principle (OCP)
- **Adapter**: Add adapters without modifying existing code
- **Decorator**: Add decorators without changing component
- **Bridge**: Extend either hierarchy independently
- **Composite**: Add new component types without changing existing ones

### Liskov Substitution Principle (LSP)
- **Adapter**: Adapter substitutes for target interface
- **Decorator**: Decorated object substitutes for component
- **Composite**: Leaf and Composite substitute for Component
- **Proxy**: Proxy substitutes for real subject

### Interface Segregation Principle (ISP)
- Patterns promote focused interfaces
- Clients depend on abstractions, not concrete implementations

### Dependency Inversion Principle (DIP)
- All patterns promote depending on abstractions
- **Bridge**: Both hierarchies depend on interfaces
- **Facade**: Clients depend on facade interface
- **Adapter**: Client depends on target interface

## Memory & Performance Considerations

### Memory Impact
- **Flyweight**: 🟢 Reduces memory (sharing)
- **Composite**: 🟡 Tree overhead
- **Decorator**: 🟡 Multiple wrapper objects
- **Proxy**: 🟢 Can reduce memory (virtual/caching)

### Performance Impact
- **Proxy**: Varies by type (virtual: better, remote: latency)
- **Decorator**: Slight overhead per layer
- **Facade**: Minimal (coordination only)
- **Flyweight**: CPU↑ for lookup, Memory↓ from sharing

### Thread Safety
- **Flyweight**: Immutable flyweights are thread-safe
- **Proxy**: Synchronization proxy for thread-safe access
- Consider immutability where possible
- Factory/pool access may need synchronization

## Common Pitfalls

1. **Adapter**: Don't over-adapt; sometimes changing the interface directly is simpler
2. **Bridge**: Don't use if abstraction/implementation won't vary independently
3. **Composite**: Safety vs transparency trade-off (where to put child management?)
4. **Decorator**: Order matters! Encryption then compression ≠ compression then encryption
5. **Facade**: Don't let facade become god object; keep it focused
6. **Flyweight**: Only works if most state is intrinsic (shareable)
7. **Proxy**: Don't confuse with Decorator (proxy controls access, decorator adds behavior)

## Learning Path Recommendation

1. **Start**: Adapter (simplest concept)
2. **Then**: Facade (easy to understand)
3. **Next**: Proxy (builds on adapter concept)
4. **Then**: Decorator (understand wrapping)
5. **Next**: Composite (tree structures)
6. **Then**: Bridge (most abstract)
7. **Finally**: Flyweight (optimization pattern)

---

**Note**: All examples follow C++17 standards with RAII, smart pointers, and modern idioms. Each pattern includes multiple real-world examples and emphasizes SOLID principles.
