# Modern C++ Brush-Up Guide

A compact revision guide for C++17 and newer, focused on practical C++, Linux, embedded systems, Android native development, and interviews.

## Recommended compiler command

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
    -fsanitize=address,undefined -g main.cpp -o app
```

Use `-std=c++20` or `-std=c++23` when practising newer features.

## Runnable teaching examples

Study the files in this order:

| File | What it teaches |
|---|---|
| [`cpp17.cpp`](cpp17.cpp) | Main lesson: references, functions, containers, algorithms, lambdas, classes, polymorphism, RAII, smart pointers, moves, optional, variant, string_view, structured bindings, and templates |
| [`array.c`](array.c) | C array fundamentals, traversal, multiplication, searching, reversing, insertion, deletion, and moving zeroes |
| [`pointers.c`](pointers.c) | Addresses, dereferencing, pointer arithmetic, pointer-to-pointer, const pointers, and dynamic memory allocation |
| [`dsa.c`](dsa.c) | Complexity, searching, sorting, stack, circular queue, and linked list |

The project is primarily for modern C++, so start with `cpp17.cpp`. The C files strengthen the memory, pointer, array, and DSA foundations used by systems-level C++.

Compile and run every example:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror cpp17.cpp -o cpp17
gcc -std=c17 -Wall -Wextra -Wpedantic -Werror array.c -o array
gcc -std=c17 -Wall -Wextra -Wpedantic -Werror pointers.c -o pointers
gcc -std=c17 -Wall -Wextra -Wpedantic -Werror dsa.c -o dsa

./cpp17
./array
./pointers
./dsa
```

---

## 1. Core language essentials

### Initialization

Prefer brace initialization because it prevents narrowing conversions.

```cpp
int count{10};
double rate{2.5};
std::string name{"Imran"};

int x = 3.9; // allowed: becomes 3
// int y{3.9}; // compile error: narrowing
```

Common forms:

```cpp
int a;        // default initialization; uninitialized for local fundamental types
int b{};      // value initialization; b is 0
int c{10};    // direct-list initialization
int d = 10;   // copy initialization
```

### `auto`, `const`, and references

```cpp
const int limit{100};

int value{10};
int& ref = value;             // mutable reference
const int& readOnly = value;  // read-only reference
auto copy = value;            // int
auto& alias = value;          // int&
const auto& safeAlias = value;
```

Use `const` by default for values that should not change.

### Range-based loops

```cpp
std::vector<int> values{1, 2, 3};

for (int value : values) { }             // copies each element
for (int& value : values) { value *= 2; } // modifies elements
for (const int& value : values) { }      // no copy, read-only
```

For small primitive values such as `int`, copying is usually fine. For large objects, prefer `const auto&`.

### Scope and lifetime

- Automatic objects are destroyed when their scope ends.
- Static objects live until program termination.
- Dynamic objects live until explicitly released or their owning smart pointer is destroyed.
- A reference or pointer must never outlive the object it refers to.
- Returning a reference or pointer to a local variable creates a dangling reference/pointer.

```cpp
const std::string& bad()
{
    std::string text{"temporary"};
    return text; // wrong: text dies when the function returns
}
```

---

## 2. Functions

### Pass parameters intentionally

```cpp
void inspect(int value);                    // copy small value
void inspect(const std::string& text);       // read without copying
void update(std::string& text);              // modify caller's object
void consume(std::string text);              // take/copy/move ownership of value
void consume(std::unique_ptr<Resource> ptr); // transfer exclusive ownership
```

### Return values

Return objects by value. Modern compilers use copy elision or moves.

```cpp
std::vector<int> makeValues()
{
    return {1, 2, 3};
}
```

### Default arguments and overloading

```cpp
void log(std::string_view message, int level = 1);
void print(int value);
void print(double value);
```

Avoid ambiguous overloads and place default arguments in declarations, normally in header files.

### Lambda expressions

```cpp
int offset{5};

auto add = [offset](int value) {
    return value + offset;
};

auto increment = [&offset] {
    ++offset;
};
```

Capture guide:

- `[]`: capture nothing
- `[x]`: capture `x` by value
- `[&x]`: capture `x` by reference
- `[=]`: capture used local variables by value
- `[&]`: capture used local variables by reference
- `[item = std::move(obj)]`: init-capture and move into lambda

Be careful when a lambda outlives values captured by reference.

---

## 3. Classes and object-oriented C++

### Basic class

```cpp
class Device {
public:
    explicit Device(std::string name)
        : name_{std::move(name)}
    {
    }

    const std::string& name() const noexcept
    {
        return name_;
    }

private:
    std::string name_;
};
```

Remember:

- Initialize members using the member-initializer list.
- Members initialize in declaration order, not list order.
- Mark single-argument constructors `explicit` unless implicit conversion is intended.
- A `const` member function cannot modify ordinary data members.
- Use `noexcept` when a function is guaranteed not to throw.
- Keep data private and preserve class invariants.

### Inheritance and virtual functions

```cpp
class Backend {
public:
    virtual ~Backend() = default;
    virtual void run() = 0;
};

class CpuBackend final : public Backend {
public:
    void run() override { }
};
```

Rules:

- A polymorphic base class normally needs a virtual destructor.
- Use `override` for overridden functions.
- Use `final` when further inheritance/overriding is not intended.
- Prefer composition over inheritance unless there is a real “is-a” relationship.
- Avoid object slicing: pass polymorphic objects by reference or pointer.

### Rule of zero, three, and five

Prefer the **Rule of Zero**: use standard containers, strings, and smart pointers so the compiler-generated special members are correct.

Resource-owning classes may need:

- Destructor
- Copy constructor
- Copy-assignment operator
- Move constructor
- Move-assignment operator

```cpp
class Buffer {
public:
    explicit Buffer(std::size_t size)
        : data_{std::make_unique<std::byte[]>(size)}, size_{size}
    {
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;

private:
    std::unique_ptr<std::byte[]> data_;
    std::size_t size_{};
};
```

---

## 4. RAII and resource management

**RAII:** acquire a resource in an object's constructor and release it in its destructor.

This applies to:

- Heap memory
- Files and file descriptors
- Mutexes
- Sockets
- Mapped memory
- Threads
- Device handles

```cpp
std::mutex mutex;

void updateSharedState()
{
    std::lock_guard<std::mutex> lock{mutex};
    // mutex automatically unlocks when lock leaves scope
}
```

For Linux file descriptors, create a small RAII owner instead of manually closing on every return path.

### Smart pointers

```cpp
auto exclusive = std::make_unique<Device>("NPU");
auto shared = std::make_shared<Device>("GPU");
std::weak_ptr<Device> observer = shared;
```

- `std::unique_ptr<T>`: one owner; preferred default
- `std::shared_ptr<T>`: shared ownership through reference counting
- `std::weak_ptr<T>`: non-owning observer of an object managed by `shared_ptr`
- Raw pointer/reference: normally non-owning access

Avoid `shared_ptr` unless shared lifetime is genuinely required. Cycles of `shared_ptr` leak memory; break cycles with `weak_ptr`.

---

## 5. Copying, moving, and value categories

```cpp
std::string source{"model"};
std::string copied = source;            // copy
std::string moved = std::move(source);  // move
```

Important:

- `std::move` does not move anything by itself; it allows move operations.
- A moved-from object remains valid but its value is generally unspecified.
- Do not assume moved-from containers or strings are empty.
- Moving a `const` object usually performs a copy because move operations normally require a mutable rvalue.
- Mark move constructors `noexcept` when possible so containers can move elements safely.

### Lvalue and rvalue references

- Lvalue: has identity and normally persists beyond the expression.
- Rvalue: temporary value or object explicitly made movable.
- `T&`: lvalue reference
- `const T&`: can bind to lvalues and temporaries
- `T&&`: rvalue reference
- `std::forward<T>`: preserves value category in forwarding code

---

## 6. Standard library containers

| Container | Best use | Important property |
|---|---|---|
| `std::array` | Fixed-size contiguous data | Size known at compile time |
| `std::vector` | Default dynamic sequence | Contiguous, cache friendly |
| `std::deque` | Fast operations at both ends | Not fully contiguous |
| `std::list` | Stable iterators and frequent middle insertion | Poor cache locality |
| `std::map` | Ordered key-value data | Usually O(log n) |
| `std::unordered_map` | Fast average key lookup | Average O(1), unordered |
| `std::set` | Ordered unique values | Usually O(log n) |
| `std::unordered_set` | Fast average membership test | Average O(1), unordered |
| `std::span` (C++20) | Non-owning contiguous view | No allocation or ownership |

Prefer `std::vector` unless another container has a clear advantage.

### Iterator invalidation

Know when container operations invalidate pointers, references, and iterators.

For `std::vector`:

- Reallocation invalidates all pointers, references, and iterators.
- Erasing invalidates the erased element and elements after it.
- `reserve()` can reduce reallocations.
- `resize()` changes element count; `reserve()` changes capacity only.

---

## 7. Algorithms instead of manual loops

```cpp
std::vector<int> values{4, 1, 3, 2};

std::sort(values.begin(), values.end());

const auto found = std::find(values.begin(), values.end(), 3);

const int evenCount = static_cast<int>(
    std::count_if(values.begin(), values.end(),
                  [](int value) { return value % 2 == 0; })
);
```

Useful algorithms:

- `find`, `find_if`
- `sort`, `stable_sort`
- `transform`
- `count`, `count_if`
- `all_of`, `any_of`, `none_of`
- `accumulate`
- `copy`, `move`
- `remove_if` with `erase`

C++20 ranges improve readability:

```cpp
std::ranges::sort(values);
const auto it = std::ranges::find(values, 3);
```

---

## 8. Error handling

Choose an error strategy deliberately.

### Exceptions

Good for failures that cannot be handled locally in exception-enabled applications.

```cpp
if (!valid) {
    throw std::runtime_error{"invalid configuration"};
}
```

### `std::optional`

Use when a result may simply be absent.

```cpp
std::optional<int> findId(std::string_view name);
```

### `std::expected` (C++23)

Use when callers need either a value or structured error information.

```cpp
std::expected<Config, Error> loadConfig();
```

### Embedded/native considerations

Many embedded or Android platform components disable exceptions and RTTI. Check the build configuration before using:

- `throw`, `try`, `catch`
- `dynamic_cast`
- `typeid`

Never ignore error-return values from system calls. Preserve `errno` when necessary and handle partial reads/writes and interruptions.

---

## 9. Templates and compile-time programming

### Function template

```cpp
template <typename T>
T maximum(const T& a, const T& b)
{
    return a < b ? b : a;
}
```

### `if constexpr` (C++17)

```cpp
template <typename T>
void printValue(const T& value)
{
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integer: " << value;
    } else {
        std::cout << value;
    }
}
```

Only the selected branch is instantiated.

### Concepts (C++20)

```cpp
template <std::integral T>
T add(T a, T b)
{
    return a + b;
}
```

Concepts express template requirements and improve compiler diagnostics.

---

## 10. Important C++17 features

### Structured bindings

```cpp
std::map<std::string, int> scores{{"A", 10}, {"B", 20}};

for (const auto& [name, score] : scores) {
    std::cout << name << ": " << score << '\n';
}
```

### `std::optional`

```cpp
std::optional<int> value = 42;

if (value) {
    std::cout << *value;
}

const int result = value.value_or(0);
```

### `std::variant`

Type-safe union:

```cpp
std::variant<int, std::string> result{"ready"};

std::visit([](const auto& value) {
    std::cout << value;
}, result);
```

### `std::string_view`

Non-owning read-only view of characters:

```cpp
void log(std::string_view message);
```

The referenced characters must outlive the view. Do not store a view of a temporary string.

### Other C++17 items

- `if` and `switch` initializers
- Inline variables
- Fold expressions
- Class template argument deduction
- `std::filesystem`
- `std::byte`
- Guaranteed copy elision in specified cases
- `[[nodiscard]]`, `[[maybe_unused]]`, `[[fallthrough]]`

---

## 11. Key C++20 and C++23 additions

### C++20

- Concepts and `requires`
- Ranges
- `std::span`
- Coroutines
- `std::jthread` and `std::stop_token`
- Three-way comparison `<=>`
- Designated initializers with C++ restrictions
- `consteval` and expanded `constexpr`
- `std::format` (library support depends on toolchain)
- Modules (toolchain/build-system support varies)

### C++23

- `std::expected`
- `std::print` and `std::println`
- More ranges utilities
- `std::mdspan`
- `if consteval`
- Deducing `this`
- Expanded `constexpr` support

Check compiler and standard-library support before using recent library features in production.

---

## 12. Concurrency

### Thread with safe cleanup

```cpp
std::jthread worker{[](std::stop_token stop) {
    while (!stop.stop_requested()) {
        // work
    }
}}; // joins automatically (C++20)
```

With `std::thread`, always join or detach before destruction. Prefer joining.

### Mutex

```cpp
std::mutex mutex;
int sharedValue{};

void increment()
{
    std::scoped_lock lock{mutex};
    ++sharedValue;
}
```

- A data race is undefined behavior.
- Protect shared mutable state.
- Keep critical sections small.
- Use `std::scoped_lock` to lock multiple mutexes safely.
- Use `std::condition_variable` for waiting on state changes, always with a predicate.
- Atomics do not automatically make a multi-step algorithm correct.
- Understand acquire/release ordering before weakening the default sequential consistency.

```cpp
condition.wait(lock, [] { return ready; });
```

The predicate handles spurious wakeups.

---

## 13. Memory and undefined behavior

Watch for:

- Out-of-bounds access
- Use after free
- Double delete/free
- Uninitialized reads
- Signed integer overflow
- Invalid shifts
- Dangling pointers/references/views
- Data races
- Incorrect casts
- Violating object lifetime or strict-aliasing rules

Prefer:

- Containers instead of raw dynamic arrays
- `std::unique_ptr` instead of `new`/`delete`
- `std::span` for pointer-plus-length parameters
- `std::byte` for raw byte buffers
- Checked sizes before arithmetic and allocation
- Sanitizers during development

Useful tools:

```bash
# AddressSanitizer + UndefinedBehaviorSanitizer
-fsanitize=address,undefined

# ThreadSanitizer (run separately)
-fsanitize=thread

# libstdc++ debug iterators
-D_GLIBCXX_DEBUG
```

---

## 14. C++ and C/system APIs

### Prefer fixed-width integers for protocols and hardware formats

```cpp
std::uint32_t registerValue{};
std::int64_t timestampNs{};
```

Use `std::size_t` for sizes and indexes. Be careful mixing signed and unsigned values.

### Casts

- `static_cast`: explicit checked-at-compile-time conversion
- `dynamic_cast`: checked polymorphic conversion; requires RTTI
- `const_cast`: add/remove constness; rarely appropriate
- `reinterpret_cast`: low-level reinterpretation; use with extreme care

Avoid C-style casts because they hide which conversion is occurring.

### C interoperability

```cpp
extern "C" {
#include <some_c_api.h>
}
```

Match allocation and release APIs:

- `new` with `delete`
- `new[]` with `delete[]`
- `malloc` with `free`
- API-specific acquire with its corresponding release

Wrap C handles in RAII objects.

---

## 15. Headers and build hygiene

Header rules:

- Use include guards or `#pragma once`.
- Include what the file uses.
- Avoid `using namespace std;` in headers.
- Prefer forward declarations when appropriate.
- Keep non-template definitions in source files.
- Templates normally need definitions visible in headers.
- Avoid macros when `constexpr`, inline functions, templates, or enums work.
- Make ownership and nullability clear in interfaces.

Example project:

```text
cpp-study/
├── README.md
├── cpp17.cpp
├── array.c
├── pointers.c
└── dsa.c
```

Minimal CMake:

```cmake
cmake_minimum_required(VERSION 3.16)
project(cpp_study LANGUAGES CXX)

add_executable(cpp_study src/main.cpp)
target_compile_features(cpp_study PRIVATE cxx_std_17)
target_compile_options(cpp_study PRIVATE -Wall -Wextra -Wpedantic)
```

---

## 16. Performance points

- Measure before optimizing.
- Prefer contiguous storage for cache locality.
- Avoid unnecessary allocation and copying.
- Pass large read-only objects by `const&` or a suitable view.
- Use `reserve()` when the approximate final size is known.
- Prefer `emplace_back` only when constructing directly in the container; `push_back` is fine for existing objects.
- Understand algorithmic complexity.
- Avoid premature use of `shared_ptr`, virtual dispatch, or complex templates.
- Move semantics do not guarantee zero cost.
- Compiler optimization cannot fix an inefficient design.

For embedded/real-time paths, also consider:

- Allocation predictability
- Lock contention
- Priority inversion
- Blocking system calls
- Worst-case latency
- Cache and memory-bandwidth behavior
- Exception and RTTI policy

---

## 17. Common interview traps

1. `delete` versus `delete[]`
2. Stack versus heap and object lifetime
3. Shallow versus deep copy
4. Copy constructor versus copy assignment
5. Virtual destructor requirement
6. Object slicing
7. Dangling pointer/reference/`string_view`
8. `reserve()` versus `resize()`
9. `push_back()` versus `emplace_back()`
10. `map` versus `unordered_map`
11. `unique_ptr` versus `shared_ptr`
12. `const T*`, `T* const`, and `const T* const`
13. `std::move` and moved-from state
14. Iterator invalidation
15. Data races and undefined behavior
16. `volatile` is not a thread-synchronization mechanism
17. Why constructors should use initializer lists
18. Why base destructors may need to be virtual
19. Rule of Zero/Five
20. RAII for non-memory resources

---

## 18. Fast review checklist

Before writing or reviewing C++ code, ask:

- Who owns each resource?
- When is each object destroyed?
- Can any pointer, reference, iterator, span, or view dangle?
- Is copying intentional?
- Can ownership be represented with `unique_ptr`?
- Is a raw pointer non-owning and is that clear?
- Are all return values and error paths handled?
- Are sizes and integer conversions safe?
- Can a container operation invalidate an iterator?
- Is shared state synchronized?
- Can a thread, callback, or lambda outlive captured objects?
- Does the base class need a virtual destructor?
- Can the Rule of Zero replace manual resource management?
- Are headers self-contained?
- Do warnings, tests, and sanitizers pass?
- Is the implementation correct before being optimized?

---

## 19. Suggested seven-day brush-up plan

| Day | Topics | Practice |
|---|---|---|
| 1 | Initialization, references, const, functions | Write small parameter-passing examples |
| 2 | Classes, special members, inheritance | Build a small RAII handle wrapper |
| 3 | STL containers and algorithms | Solve vector/map transformation tasks |
| 4 | Smart pointers and move semantics | Model exclusive and shared ownership |
| 5 | C++17 vocabulary types and templates | Use optional, variant, string_view, if constexpr |
| 6 | Threads, mutexes, condition variables | Implement a producer-consumer queue |
| 7 | C++20/23 overview and review | Add span, ranges, concepts, expected |

Repeat with small executable examples. Reading alone is not enough—compile with warnings and sanitizers.

## 20. Final one-minute summary

```text
C++ object lifetime
    -> RAII owns resources
    -> Rule of Zero avoids manual cleanup
    -> unique_ptr is the default owning pointer
    -> const/reference/value express intent
    -> vector is the default container
    -> algorithms reduce error-prone loops
    -> move transfers reusable resources
    -> optional/variant/expected model outcomes safely
    -> mutex/atomic rules prevent data races
    -> warnings + tests + sanitizers catch mistakes
```
