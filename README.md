# cppunittest
A minimal unit testing framework for C++23.

# Assertion macros

- `CHECK(expr)`: Ensure `static_cast<bool>(expr)` is `true`
- `CHECK_EQUAL(lhs, rhs)`: Ensure `(lhs == rhs)` is `true`
- `CHECK_THROWS_AS(T, expr)`: Ensure `expr` throws an exception of type `T`

The above macros have fatal variants that terminate the currently running test upon failure: `REQUIRE`, `REQUIRE_EQUAL`, `REQUIRE_THROWS_AS`

# Example usage

```cpp
#include <cppunittest/unittest.hpp>
#include <ranges>
#include <vector>

UNITTEST("vector push_back")
{
    std::vector<int> vec;
    REQUIRE(vec.empty());
    REQUIRE_THROWS_AS(std::out_of_range, (void)vec.at(0));

    vec.push_back(5);
    REQUIRE_EQUAL(vec.size(), 1uz);
    REQUIRE_EQUAL(vec.at(0), 5);
    REQUIRE_THROWS_AS(std::out_of_range, (void)vec.at(1));

    vec.push_back(10);
    REQUIRE_EQUAL(vec.size(), 2uz);
    REQUIRE_EQUAL(vec.at(0), 5);
    REQUIRE_EQUAL(vec.at(1), 10);
    REQUIRE_THROWS_AS(std::out_of_range, (void)vec.at(2));
}

UNITTEST("vector from range")
{
    auto const square = [](auto const x) { return x * x; };

    std::vector<int> const vec
        = std::views::iota(1)             // Unbounded sequence 1,2,3...
        | std::views::transform(square)   // Square each integer in the sequence
        | std::views::take(10)            // Only take the first 10
        | std::ranges::to<std::vector>(); // Construct a vector from the range

    REQUIRE_EQUAL(vec, std::vector { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 });
}
```
