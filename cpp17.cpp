#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

/*
 * MODERN C++17 BEGINNER LESSON
 * ============================
 * Compile:
 *   g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror cpp17.cpp -o cpp17
 *
 * This file teaches C++ by running small examples in order.
 */

void heading(std::string_view text)
{
    std::cout << "\n--- " << text << " ---\n";
}

/* 1. INITIALIZATION, AUTO, CONST, REFERENCES */
void basics()
{
    int number{10};             // brace initialization
    const int limit{100};       // cannot be modified
    auto another{20};           // compiler deduces int
    int& reference{number};     // alias for number
    const int& read_only{number};

    reference = 25;             // changes number
    std::cout << "number=" << number
              << " limit=" << limit
              << " auto=" << another
              << " const-reference=" << read_only << '\n';
}

/* 2. PASS BY VALUE, REFERENCE, AND CONST REFERENCE */
void by_value(int value)
{
    value = 99; // changes only this local copy
    std::cout << "inside by_value=" << value << '\n';
}
void by_reference(int& value) { value = 99; }    // changes caller
void inspect(const std::string& text)             // no copy, cannot modify
{
    std::cout << "text=" << text << " size=" << text.size() << '\n';
}

void parameter_demo()
{
    int first{10};
    int second{10};
    by_value(first);
    by_reference(second);
    std::cout << "after by_value=" << first
              << " after by_reference=" << second << '\n';
    inspect("C++17");
}

/* 3. STD::ARRAY AND STD::VECTOR */
void container_demo()
{
    std::array<int, 4> fixed{10, 20, 30, 40}; // fixed size
    std::vector<int> dynamic{1, 2, 3};        // dynamic size

    dynamic.push_back(4);
    dynamic.emplace_back(5);
    dynamic.reserve(10); // capacity only; size is still 5

    fixed.at(0) = 11;    // at() checks bounds
    dynamic[0] = 10;     // [] does not check bounds

    std::cout << "array:";
    for (const int value : fixed) std::cout << ' ' << value;

    std::cout << "\nvector:";
    for (const int value : dynamic) std::cout << ' ' << value;
    std::cout << "\nsize=" << dynamic.size()
              << " capacity=" << dynamic.capacity() << '\n';
}

/* 4. ARRAY MULTIPLICATION USING A LOOP AND AN ALGORITHM */
std::vector<int> multiply_loop(const std::vector<int>& input, int factor)
{
    std::vector<int> result;
    result.reserve(input.size());
    for (const int value : input) result.push_back(value * factor);
    return result;
}

std::vector<int> multiply_algorithm(const std::vector<int>& input, int factor)
{
    std::vector<int> result(input.size());
    std::transform(input.begin(), input.end(), result.begin(),
                   [factor](int value) { return value * factor; });
    return result;
}

void algorithm_demo()
{
    const std::vector<int> values{5, 1, 4, 2, 3};
    auto multiplied = multiply_algorithm(values, 3);
    std::sort(multiplied.begin(), multiplied.end());

    const int total =
        std::accumulate(multiplied.begin(), multiplied.end(), 0);
    const auto even_count =
        std::count_if(multiplied.begin(), multiplied.end(),
                      [](int value) { return value % 2 == 0; });

    std::cout << "sorted x3:";
    for (const int value : multiplied) std::cout << ' ' << value;
    std::cout << "\nsum=" << total << " even-count=" << even_count << '\n';

    const auto loop_result = multiply_loop(values, 2);
    std::cout << "loop x2:";
    for (const int value : loop_result) std::cout << ' ' << value;
    std::cout << '\n';
}

/* 5. LAMBDAS AND CAPTURE */
void lambda_demo()
{
    int factor{3};
    auto multiply_by_value = [factor](int value) { return value * factor; };
    auto change_factor = [&factor] { factor = 5; };

    std::cout << "4 x captured 3 = " << multiply_by_value(4) << '\n';
    change_factor();
    std::cout << "factor changed through reference capture to " << factor
              << '\n';
    /* multiply_by_value still captured its own old copy: 3. */
}

/* 6. CLASS, CONSTRUCTOR, ENCAPSULATION, CONST MEMBER */
class Device {
public:
    Device(std::string name, int id)
        : name_{std::move(name)}, id_{id}
    {
        std::cout << "Constructed " << name_ << '\n';
    }

    ~Device() { std::cout << "Destroyed " << name_ << '\n'; }

    const std::string& name() const noexcept { return name_; }
    int id() const noexcept { return id_; }
    void set_id(int id) noexcept { id_ = id; }

private:
    std::string name_;
    int id_{};
};

void class_demo()
{
    Device device{"NPU", 7};
    device.set_id(8);
    std::cout << "device=" << device.name() << " id=" << device.id() << '\n';
} // destructor runs automatically here

/* 7. POLYMORPHISM */
class Backend {
public:
    virtual ~Backend() = default; // required for deletion through base pointer
    virtual std::string_view type() const noexcept = 0;
    virtual void run() const = 0;
};

class CpuBackend final : public Backend {
public:
    std::string_view type() const noexcept override { return "CPU"; }
    void run() const override { std::cout << "Running on CPU\n"; }
};

void polymorphism_demo()
{
    std::unique_ptr<Backend> backend = std::make_unique<CpuBackend>();
    std::cout << "backend type=" << backend->type() << '\n';
    backend->run();
}

/* 8. RAII AND SMART POINTERS */
class Resource {
public:
    explicit Resource(std::string name) : name_{std::move(name)}
    {
        std::cout << "Acquired " << name_ << '\n';
    }

    ~Resource() { std::cout << "Released " << name_ << '\n'; }

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) noexcept = default;
    Resource& operator=(Resource&&) noexcept = default;

private:
    std::string name_;
};

void smart_pointer_demo()
{
    auto exclusive = std::make_unique<Resource>("exclusive resource");
    auto shared1 = std::make_shared<Resource>("shared resource");
    std::weak_ptr<Resource> observer = shared1;

    {
        auto shared2 = shared1;
        std::cout << "shared owners=" << shared1.use_count() << '\n';
    }
    std::cout << "shared owners after scope=" << shared1.use_count() << '\n';
    std::cout << "weak observer expired=" << std::boolalpha
              << observer.expired() << '\n';
}

/* 9. COPY AND MOVE */
class Buffer {
public:
    explicit Buffer(std::size_t size)
        : data_{std::make_unique<int[]>(size)}, size_{size}
    {
    }

    Buffer(const Buffer&) = delete;            // no copying
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) noexcept = default;        // ownership can move
    Buffer& operator=(Buffer&&) noexcept = default;

    std::size_t size() const noexcept { return size_; }

private:
    std::unique_ptr<int[]> data_;
    std::size_t size_{};
};

void move_demo()
{
    std::string source{"model-data"};
    std::string destination{std::move(source)};
    std::cout << "moved destination=" << destination << '\n';
    /* source remains valid but its value is unspecified; it may be reused. */

    Buffer first{128};
    Buffer second{std::move(first)};
    std::cout << "moved buffer size=" << second.size() << '\n';
}

/* 10. OPTIONAL: A RESULT MAY BE ABSENT */
std::optional<int> first_even(const std::vector<int>& values)
{
    const auto found =
        std::find_if(values.begin(), values.end(),
                     [](int value) { return value % 2 == 0; });
    if (found == values.end()) return std::nullopt;
    return *found;
}

void optional_demo()
{
    if (const auto result = first_even({1, 3, 6, 7}); result.has_value())
        std::cout << "first even=" << *result << '\n';

    const auto absent = first_even({1, 3, 5});
    std::cout << "fallback=" << absent.value_or(-1) << '\n';
}

/* 11. VARIANT: TYPE-SAFE CHOICE BETWEEN TYPES */
using Result = std::variant<int, std::string>;

void print_result(const Result& result)
{
    std::visit([](const auto& value) { std::cout << "result=" << value << '\n'; },
               result);
}

void variant_demo()
{
    Result result{42};
    print_result(result);
    result = std::string{"ready"};
    print_result(result);
}

/* 12. STRING_VIEW: NON-OWNING CHARACTER VIEW */
void show_text(std::string_view text)
{
    std::cout << "view='" << text << "' length=" << text.size() << '\n';
}

void string_view_demo()
{
    std::string owned{"GenieX"};
    show_text(owned);
    show_text("literal");
    /* Never store a string_view longer than the characters it views live. */
}

/* 13. STRUCTURED BINDINGS */
void structured_binding_demo()
{
    const std::map<std::string, int> scores{
        {"C++", 90}, {"Linux", 85}, {"Android", 88}
    };

    for (const auto& [topic, score] : scores)
        std::cout << topic << " -> " << score << '\n';
}

/* 14. TEMPLATES AND IF CONSTEXPR */
template <typename T>
T maximum(const T& left, const T& right)
{
    return left < right ? right : left;
}

template <typename T>
void describe(const T& value)
{
    if constexpr (std::is_integral_v<T>)
        std::cout << value << " is an integral value\n";
    else
        std::cout << value << " is a non-integral value\n";
}

void template_demo()
{
    std::cout << "maximum int=" << maximum(10, 20) << '\n';
    std::cout << "maximum string="
              << maximum(std::string{"CPU"}, std::string{"NPU"}) << '\n';
    describe(42);
    describe(3.14);
}

int main()
{
    heading("1. Initialization, auto, const, references"); basics();
    heading("2. Function parameters"); parameter_demo();
    heading("3. array and vector"); container_demo();
    heading("4. Algorithms and array multiplication"); algorithm_demo();
    heading("5. Lambdas"); lambda_demo();
    heading("6. Classes and RAII lifetime"); class_demo();
    heading("7. Runtime polymorphism"); polymorphism_demo();
    heading("8. Smart pointers"); smart_pointer_demo();
    heading("9. Copy and move"); move_demo();
    heading("10. optional"); optional_demo();
    heading("11. variant and visit"); variant_demo();
    heading("12. string_view"); string_view_demo();
    heading("13. Structured bindings"); structured_binding_demo();
    heading("14. Templates and if constexpr"); template_demo();
    return 0;
}
