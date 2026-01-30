# Behavioral Design Patterns

**Behavioral patterns** focus on communication between objects and responsibility distribution. They deal with **object collaboration** and the **delegation of responsibilities**.

### Key Characteristics:
- Define **how objects interact** and distribute responsibility
- Establish clear communication patterns
- Describe object **behavior and communication**
- Make systems **loosely coupled** and **highly cohesive**

---

## All 11 Behavioral Patterns

### 1. **Chain of Responsibility** ✓
- **File:** `01_chain_of_responsibility_pattern.cpp`
- **Intent:** Pass requests along a chain of handlers
- **Problem:** Multiple handlers; don't know in advance which will handle request
- **Solution:** Each handler decides to handle or pass to next
- **Use Cases:** Support ticket routing, logger levels, approval workflows
- **Key Concept:** Decoupled sender, flexible handler chain

### 2. **Command** ✓
- **File:** `02_command_pattern.cpp`
- **Intent:** Encapsulate requests as objects
- **Problem:** Decouple sender from receivers; need undo/redo/queuing
- **Solution:** Wrap request in command object with execute/undo
- **Use Cases:** Undo/redo, macro recording, job queues, transactions
- **Key Concept:** Action objects, parameterized operations

### 3. **Iterator** ✓
- **File:** `03_iterator_pattern.cpp`
- **Intent:** Access elements sequentially without exposing structure
- **Problem:** Different collections need different traversal logic
- **Solution:** Uniform iterator interface for all collections
- **Use Cases:** Container traversal, cursor in databases, file iteration
- **Key Concept:** Standardized sequential access

### 4. **Observer** ✓
- **File:** `04_observer_pattern.cpp`
- **Intent:** Define one-to-many dependency; notify observers automatically
- **Problem:** Multiple objects need notification when subject changes
- **Solution:** Observers subscribe; subject broadcasts changes
- **Use Cases:** Event systems, MVC, reactive programming, Pub-Sub
- **Key Concept:** Automatic notification, loose coupling

### 5. **State** ✓
- **File:** `05_state_pattern.cpp`
- **Intent:** Allow object to alter behavior when internal state changes
- **Problem:** Behavior changes with state; giant if-else chains
- **Solution:** Each state is separate class; context delegates
- **Use Cases:** State machines, TCP connections, game states, workflows
- **Key Concept:** State-specific behavior, automatic transitions

### 6. **Strategy** ✓
- **File:** `06_strategy_pattern.cpp`
- **Intent:** Define family of algorithms; make them interchangeable
- **Problem:** Multiple algorithms; can't hardcode one
- **Solution:** Each algorithm is separate class; client chooses
- **Use Cases:** Sorting algorithms, payment methods, compression algorithms
- **Key Concept:** Algorithm encapsulation, runtime selection

### 7. **Mediator** ✓
- **File:** `07_mediator_pattern.cpp`
- **Intent:** Centralize complex communications between objects
- **Problem:** Objects have complex interactions; tight coupling
- **Solution:** Mediator handles all communication
- **Use Cases:** Air traffic control, chat systems, UI components, event routing
- **Key Concept:** Centralized communication, loose coupling

### 8. **Template Method** ✓
- **File:** `08_template_method_pattern.cpp`
- **Intent:** Define algorithm skeleton; subclasses override specific steps
- **Problem:** Multiple algorithms with similar structure but different details
- **Solution:** Base class defines structure; subclasses implement steps
- **Use Cases:** Frameworks, data processing, report generation, game AI
- **Key Concept:** Code reuse, enforced structure, hook methods

### 9. **Visitor** ✓
- **File:** `09_visitor_pattern.cpp`
- **Intent:** Represent operations on object structure without changing classes
- **Problem:** Many operations on complex structures; modifying classes is hard
- **Solution:** Operations visit elements; elements accept visitors
- **Use Cases:** Compilers, tax systems, file operations, report generators
- **Key Concept:** External operations, double dispatch, working with Composite

### 10. **Memento** ✓
- **File:** `10_memento_pattern.cpp`
- **Intent:** Capture and externalize state without violating encapsulation
- **Problem:** Save/restore state without exposing internals
- **Solution:** Memento captures state; originator restores from it
- **Use Cases:** Undo/redo, game checkpoints, database transactions, rollback
- **Key Concept:** State snapshots, preserved encapsulation

### 11. **Interpreter** ✓
- **File:** `11_interpreter_pattern.cpp`
- **Intent:** Define representation for language grammar; provide interpreter
- **Problem:** Need to parse/interpret simple language or expressions
- **Solution:** Grammar as classes; interpreter evaluates expressions
- **Use Cases:** SQL parsers, expression evaluators, query languages, DSLs
- **Key Concept:** AST (Abstract Syntax Tree), recursive interpretation
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
