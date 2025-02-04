# cppunittest
A minimal unit testing framework for C++23.

# Assertion macros

- `CHECK(expr)`: Ensure `static_cast<bool>(expr)` is `true`
- `CHECK_EQUAL(lhs, rhs)`: Ensure `(lhs == rhs)` is `true`
- `CHECK_NOT_EQUAL(lhs, rhs)`: Ensure `(lhs == rhs)` is `false`
- `CHECK_THROWS_AS(T, expr)`: Ensure `expr` throws an exception of type `T`

The above macros have fatal variants that terminate the currently running test upon failure: `REQUIRE`, `REQUIRE_EQUAL`, `REQUIRE_NOT_EQUAL`, `REQUIRE_THROWS_AS`

# Example usage

```cpp
#include <cppunittest/unittest.hpp>
#include <limits>
#include <ranges>
#include <vector>

UNITTEST("vector push_back")
{
    std::vector<int> vec;
    CHECK(vec.empty());

    vec.push_back(5);
    CHECK_EQUAL(vec.size(), 1uz);
    CHECK_EQUAL(vec.front(), 5);
    CHECK_EQUAL(vec.back(), 5);

    vec.push_back(10);
    CHECK_EQUAL(vec.size(), 2uz);
    CHECK_EQUAL(vec.front(), 5);
    CHECK_EQUAL(vec.back(), 10);
}

UNITTEST("vector at")
{
    std::vector<int> const vec { 10, 20 };
    CHECK_EQUAL(vec.at(0), 10);
    CHECK_EQUAL(vec.at(1), 20);
    CHECK_THROWS_AS(std::out_of_range, (void)vec.at(2));
    CHECK_THROWS_AS(std::out_of_range, (void)vec.at(std::numeric_limits<std::size_t>::max()));
}

UNITTEST("vector from range")
{
    auto const square = [](auto const x) { return x * x; };

    std::vector<int> const vec
        = std::views::iota(1)             // Unbounded sequence 1,2,3...
        | std::views::transform(square)   // Square each integer in the sequence
        | std::views::take(10)            // Only take the first 10
        | std::ranges::to<std::vector>(); // Construct a vector from the range

    CHECK_EQUAL(vec, std::vector { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 });
}
```
