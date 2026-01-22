# Design Patterns

Interview-focused design pattern implementations demonstrating common architectural solutions in C++.

## Organization

Patterns are organized in subdirectories by category following Gang of Four (GoF) classification:

```
design_principles/
├── creational/       # Object creation patterns
├── structural/       # Object composition patterns
└── behavioral/       # Object communication patterns
```

### Creational Patterns (`creational/`)
Focus on object creation mechanisms:
- **Singleton** - Ensure a class has only one instance
- **Factory Method** - Define interface for creating objects, let subclasses decide which class to instantiate
- **Abstract Factory** - Create families of related objects without specifying concrete classes
- **Builder** - Construct complex objects step by step
- **Prototype** - Clone objects without coupling to their concrete classes

### Structural Patterns (`structural/`)
Deal with object composition and relationships:
- **Adapter** - Make incompatible interfaces work together
- **Bridge** - Separate abstraction from implementation
- **Composite** - Compose objects into tree structures
- **Decorator** - Add responsibilities to objects dynamically
- **Facade** - Provide simplified interface to complex subsystems
- **Flyweight** - Share common state to support large numbers of objects
- **Proxy** - Provide placeholder/surrogate for another object

### Behavioral Patterns (`behavioral/`)
Focus on communication between objects:
- **Chain of Responsibility** - Pass requests along handler chain
- **Command** - Encapsulate requests as objects
- **Iterator** - Access elements sequentially without exposing representation
- **Mediator** - Reduce coupling between communicating objects
- **Memento** - Capture and restore object state
- **Observer** - Define one-to-many dependency for state changes
- **State** - Alter object behavior when internal state changes
- **Strategy** - Define family of algorithms, make them interchangeable
- **Template Method** - Define algorithm skeleton, let subclasses override steps
- **Visitor** - Separate algorithms from object structure

## File Naming Convention

Each pattern follows numbered examples:
- `01_<pattern>_problem.cpp` - Shows the problem without the pattern
- `02_<pattern>_solution.cpp` - Clean implementation of the pattern
- `03_<pattern>_advanced.cpp` - Real-world scenario or variations

## Build and Run

Each category folder has its own makefile:

```bash
# Navigate to category folder
cd creational/

# Compile and run specific pattern
make FILE=01_singleton_problem.cpp run

# Navigate to another category
cd ../behavioral/
make FILE=04_observer_pattern.cpp run

# Clean build artifacts
make clean
```

## Interview Focus

Each example demonstrates:
1. **Problem Statement** - Why the pattern is needed
2. **Violation** - Code without the pattern (where applicable)
3. **Correct Implementation** - Proper pattern usage
4. **Real-World Example** - Practical application
5. **Trade-offs** - When to use and when to avoid

## Common Interview Questions Covered

- When would you use X pattern over Y?
- What are the drawbacks of this pattern?
- How does this relate to SOLID principles?
- Can you implement X pattern in 10 minutes?
- What's the difference between Strategy and State?
- Why is Singleton considered an anti-pattern by some?

## Relationship to SOLID Principles

Design patterns often embody SOLID principles:
- **Strategy/State** → Open/Closed Principle
- **Factory Method** → Dependency Inversion Principle
- **Decorator** → Open/Closed + Single Responsibility
- **Observer** → Dependency Inversion Principle
- **Adapter** → Interface Segregation Principle

## Resources

- Gang of Four: "Design Patterns: Elements of Reusable Object-Oriented Software"
- Effective C++ patterns for modern C++17/20
- System design interview patterns commonly asked at FAANG
