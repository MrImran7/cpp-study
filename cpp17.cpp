#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

/*
 * Q1: Why prefer std::vector over a variable-length array?
 * A: Variable-length arrays are not standard C++. vector owns dynamic storage,
 *    knows its size, cleans itself up, and works with standard algorithms.
 */
std::vector<int> multiply(const std::vector<int>& input, int factor)
{
    std::vector<int> result;
    result.reserve(input.size());

    std::transform(input.begin(), input.end(), std::back_inserter(result),
                   [factor](int value) { return value * factor; });
    return result;
}

/* Q2: How can a function represent "no result" without a magic value? */
std::optional<int> find_even(const std::vector<int>& values)
{
    const auto found = std::find_if(values.begin(), values.end(),
                                    [](int value) { return value % 2 == 0; });
    if (found == values.end()) {
        return std::nullopt;
    }
    return *found;
}

/*
 * Q3: Why use string_view?
 * A: It provides a non-owning view and avoids a string copy. The referenced
 *    characters must outlive the view.
 */
void print_label(std::string_view label)
{
    std::cout << label << ": ";
}

class Resource {
public:
    explicit Resource(std::string name) : name_{std::move(name)} {}
    ~Resource() { std::cout << "Released " << name_ << '\n'; }

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) noexcept = default;
    Resource& operator=(Resource&&) noexcept = default;

private:
    std::string name_;
};

int main()
{
    const std::vector<int> values{1, 2, 3, 4, 5};
    const auto multiplied = multiply(values, 4);

    print_label("Multiplied");
    for (const int value : multiplied) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    if (const auto even = find_even(values); even.has_value()) {
        std::cout << "First even value: " << *even << '\n';
    }

    const int total = std::accumulate(values.begin(), values.end(), 0);
    std::cout << "Sum: " << total << '\n';

    auto resource = std::make_unique<Resource>("demo resource");
    return 0;
}
