# C++ Exception Handling

A practical path from basics to advanced exception handling in modern C++ (C++17).

## Quick Start
- Build and run any example with:

```
make FILE=01_try_catch_basics.cpp run
```

- Change `FILE=` to any example in this folder, e.g. `04_custom_exception.cpp`.

## Learning Path
- Basics: `try`, `catch`, `throw`; catch order and by-reference catches.
- Standard exceptions: `std::exception` and derived types; `what()`.
- Custom exceptions: derive from `std::exception` safely.
- Stack unwinding & RAII: deterministic cleanup on exceptions.
- Exception safety: basic vs strong guarantees; commit/rollback patterns.
- `noexcept`: specs and the `noexcept(expr)` operator; function try blocks.
- Cross-thread propagation: `std::async`, `std::exception_ptr`.
- Nested exceptions: `std::throw_with_nested`, `std::rethrow_if_nested`.
- Error codes vs exceptions: `std::error_code` vs throwing APIs (filesystem).

## Notes & Best Practices
- Prefer catching by `const&` (e.g., `const std::exception&`).
- Order catches from most specific to least (`...` last).
- Do not throw from destructors; use `noexcept` destructors.
- Use RAII for resource management; avoid `new`/`delete` directly.
- Provide clear invariants and commit points for strong guarantees.
