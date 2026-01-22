# Behavioral Design Patterns

Patterns concerned with algorithms and assignment of responsibilities between objects, dealing with object collaboration and communication.

## Patterns in This Folder

### Observer (Pub-Sub)
- **Files:** `04_observer_pattern.cpp`
- **Intent:** Define one-to-many dependency for automatic notifications
- **Use Cases:** Event systems, MVC (model notifies views), real-time feeds
- **Key Concept:** Subject maintains list of observers, notifies them of changes

### Strategy
- **Status:** Coming soon
- **Intent:** Define family of algorithms, make them interchangeable
- **Use Cases:** Sorting algorithms, payment methods, compression algorithms
- **Key Concept:** Encapsulate algorithms in separate classes

### Command
- **Status:** Coming soon
- **Intent:** Encapsulate request as object
- **Use Cases:** Undo/redo, macro recording, job queues
- **Key Concept:** Action objects with execute() method

### State
- **Status:** Coming soon
- **Intent:** Alter behavior when internal state changes
- **Use Cases:** TCP connections, vending machines, workflow engines
- **Key Concept:** State-specific behavior in state objects

### Template Method
- **Status:** Coming soon
- **Intent:** Define algorithm skeleton, let subclasses override specific steps
- **Use Cases:** Frameworks, data parsing, game AI
- **Key Concept:** Hook methods for customization

### Chain of Responsibility
- **Status:** Coming soon
- **Intent:** Pass requests along chain of handlers
- **Use Cases:** Event bubbling, logging chains, approval workflows
- **Key Concept:** Decoupled sender and receiver

### Iterator
- **Status:** Coming soon
- **Intent:** Access elements sequentially without exposing representation
- **Use Cases:** Container traversal, database cursors
- **Key Concept:** Standardized traversal interface

### Mediator
- **Status:** Coming soon
- **Intent:** Define object that encapsulates how objects interact
- **Use Cases:** Chat rooms, air traffic control, UI component coordination
- **Key Concept:** Centralized communication

### Memento
- **Status:** Coming soon
- **Intent:** Capture and restore object state without violating encapsulation
- **Use Cases:** Undo mechanisms, checkpoints, snapshots
- **Key Concept:** State externalization

### Visitor
- **Status:** Coming soon
- **Intent:** Separate algorithm from object structure it operates on
- **Use Cases:** Compilers (AST operations), document processing
- **Key Concept:** Double dispatch for operations

## Build Examples

```bash
# Observer pattern (pub-sub)
make FILE=04_observer_pattern.cpp run

# Strategy pattern
make FILE=05_strategy_pattern.cpp run
```

## Key Takeaways

**When to use Behavioral Patterns:**
- Need flexible communication between objects
- Want to vary algorithms independently
- Need to manage complex workflows
- Want to decouple sender from receiver
- Need to maintain object state history

**Common Interview Questions:**
- Observer vs Pub-Sub?
- Strategy vs State pattern?
- When to use Command pattern?
- Template Method vs Strategy?
- Chain of Responsibility drawbacks?
- Visitor pattern double dispatch?
