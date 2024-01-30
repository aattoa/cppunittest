#include <cppunittest/unittest.hpp>

#include <algorithm>
#include <cstdlib>
#include <print>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
    struct Test {
        using Run = void (*)();
        std::string_view name;
        Run              run;
    };

    constinit std::vector<Test> unittest_vector;                     // NOLINT: mutable global
    constinit int               unittest_exit_status = EXIT_SUCCESS; // NOLINT: mutable global

    auto test_failure(cppunittest::internal::Assertion_type const type) -> void
    {
        if (type == cppunittest::internal::Assertion_type::require) {
            std::quick_exit(EXIT_FAILURE);
        }
        else {
            unittest_exit_status = EXIT_FAILURE;
        }
    }

    auto print_assertion_failure(
        std::string_view const assertion, std::source_location const caller) -> void
    {
        std::println(
            stderr,
            "Assertion failed: {}, on line {} in {}",
            assertion,
            caller.line(),
            caller.file_name());
    }
} // namespace

auto cppunittest::internal::perform_assert(
    Assertion_type const       type,
    bool const                 value,
    std::string_view const     description,
    std::source_location const caller) -> void
{
    if (value) {
        return;
    }
    print_assertion_failure(description, caller);
    test_failure(type);
}

auto cppunittest::internal::perform_type_erased_assert_equals(
    Assertion_type const       type,
    bool const                 equal,
    std::string_view const     description,
    std::format_args const     format_args,
    std::source_location const caller) -> void
{
    if (equal) {
        return;
    }
    print_assertion_failure(description, caller);
    std::println(stderr, "{}", std::vformat("Left:  {}\nRight: {}", format_args));
    test_failure(type);
}

auto cppunittest::internal::list_tests() -> void
{
    for (auto const& [test_name, _] : unittest_vector) {
        std::println("{}", test_name);
    }
}

auto cppunittest::internal::run_test(std::string_view const requested_name) -> int
{
    auto const test = std::ranges::find(unittest_vector, requested_name, &Test::name);
    if (test != unittest_vector.end()) {
        unittest_exit_status = EXIT_SUCCESS;
        test->run();
        return unittest_exit_status;
    }
    throw std::runtime_error { std::format("No test named '{}' found", requested_name) };
}

cppunittest::internal::Test_adder::Test_adder(std::string_view const name, void (*const run)())
{
    if (std::ranges::contains(unittest_vector, name, &Test::name)) {
        std::println(stderr, "Error: cppunittest: Detected multiple tests named '{}'", name);
        std::quick_exit(EXIT_FAILURE);
    }
    else {
        unittest_vector.emplace_back(name, run);
    }
}
