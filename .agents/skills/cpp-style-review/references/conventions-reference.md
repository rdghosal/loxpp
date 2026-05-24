# C++ Conventions

Idiomatic C++23 guidelines for this project. Update this file as the project evolves — it is loaded automatically during style reviews.

## Ownership and memory

**Never use raw `new`/`delete`.** All heap allocation goes through RAII wrappers.

| Ownership model | C++ | Rust analogue |
|---|---|---|
| Unique ownership | `std::unique_ptr<T>` | `Box<T>` |
| Shared ownership | `std::shared_ptr<T>` | `Arc<T>` |
| Non-owning reference | `T&` or `T*` (non-owning only) | `&T` / `*const T` |

```cpp
// bad
Foo* f = new Foo();
delete f;

// good
auto f = std::make_unique<Foo>();
```

Prefer stack allocation. Heap-allocate only when lifetime or size demands it.

## Move semantics

C++ moves are opt-in; Rust moves are the default. A moved-from C++ object remains valid but in an unspecified state — do not use it after `std::move`.

```cpp
auto a = std::make_unique<Foo>();
auto b = std::move(a); // a is now nullptr — do not touch a
```

When a type should not be copied (expensive or meaningless), delete the copy constructor and copy assignment:

```cpp
Foo(const Foo&) = delete;
Foo& operator=(const Foo&) = delete;
```

Unlike Rust, C++ does not move by default — you must say `std::move` explicitly.

## const-correctness

Every variable and parameter that is not mutated must be `const`. Every member function that does not mutate state must be marked `const`.

```cpp
// bad — copies string, ignores immutability
void print(std::string s);

// good
void print(const std::string& s);
```

In Rust, immutability is the default (`let`). In C++ you must opt in — treat every `const`-less binding as a code smell until proven necessary.

Mark return values `[[nodiscard]]` when ignoring them is almost certainly a bug.

## Error handling

Prefer `std::expected<T, E>` (C++23) over exceptions for recoverable errors. Prefer `std::optional<T>` for nullable returns.

| Rust | C++ |
|---|---|
| `Result<T, E>` | `std::expected<T, E>` |
| `Option<T>` | `std::optional<T>` |
| `.unwrap()` | `.value()` — throws if empty |
| `?` operator | explicit early return |

Reserve `throw` for unrecoverable invariant violations, not normal control flow.

```cpp
// good
std::expected<Token, ScanError> scan_token();

// good
std::optional<Token> peek();
```

## Prefer algorithms over raw loops

```cpp
// bad
int sum = 0;
for (const auto& x : v) sum += x;

// good
auto sum = std::ranges::fold_left(v, 0, std::plus{});
```

Use `<algorithm>` and `<ranges>`. They express intent, are harder to get wrong, and compose cleanly. Unlike Rust's iterator adapters, C++ ranges are lazy by default in C++20+.

## Casts

Never use C-style casts `(T)x`. Use named casts:

| Cast | Use |
|---|---|
| `static_cast<T>(x)` | safe numeric/type conversions |
| `reinterpret_cast<T>(x)` | bit-level reinterpretation — rare, dangerous |
| `const_cast<T>(x)` | remove const — almost never correct |

## Initialization

Use braced initialization `{}` to prevent narrowing conversions:

```cpp
int x{42};
auto v = std::vector<int>{1, 2, 3};
```

Avoid `=` initialization for non-primitive types; it can invoke implicit conversions silently.

## Naming

Matches `.clang-tidy` config:

| Entity | Convention | Example |
|---|---|---|
| Variables | `lower_case` | `token_count` |
| Functions | `lower_case` | `scan_token()` |
| Classes / structs | `CamelCase` | `TokenScanner` |
| Namespaces | `lower_case` | `namespace lox` |
| Private members | `lower_case_` (trailing underscore) | `source_` |

## Includes and headers

Use `#pragma once` (not include guards). Include order per `.clang-format`:

1. Own header (`"lox/scanner.hpp"`)
2. Standard library (`<string>`, `<vector>`)
3. Third-party

Declare in `.hpp`, define in `.cpp`. Template definitions must live in headers (or a `_impl.hpp` included by the header).

Keep headers minimal: prefer forward declarations over full includes in `.hpp`.
