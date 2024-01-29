# cppunittest
A minimal unit testing framework for C++23.

# Assertion macros

- `CHECK(expr)`: Ensure `static_cast<bool>(expr)` is `true`
- `CHECK_EQUAL(lhs, rhs)`: Ensure `(lhs == rhs)` is `true`

The above macros have fatal variants that terminate the currently running test upon failure: `REQUIRE`, `REQUIRE_EQUAL`

# Example usage

```cpp
#include <cppunittest/unittest.hpp>
#include <ranges>
#include <vector>

UNITTEST("vector push_back")
{
    std::vector<int> vec;
    REQUIRE(vec.empty());

    vec.push_back(5);
    REQUIRE_EQUAL(vec.size(), 1uz);
    REQUIRE_EQUAL(vec.front(), 5);

    vec.push_back(10);
    REQUIRE_EQUAL(vec.size(), 2uz);
    REQUIRE_EQUAL(vec.front(), 5);
    REQUIRE_EQUAL(vec.back(), 10);
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
