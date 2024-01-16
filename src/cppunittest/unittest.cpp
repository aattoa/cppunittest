#include <cppunittest/unittest.hpp>

#include <cstdlib>
#include <print>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
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

    [[nodiscard]] auto test_container() noexcept -> auto&
    {
        static std::vector<std::pair<std::string_view, void (*)()>> container;
        return container;
    }
} // namespace

auto cppunittest::internal::require(
    bool const value, std::string_view const description, std::source_location const caller) -> void
{
    if (value) {
        return;
    }
    print_assertion_failure(description, caller);
    std::quick_exit(EXIT_FAILURE);
}

auto cppunittest::internal::do_require_equal(
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
    std::quick_exit(EXIT_FAILURE);
}

auto cppunittest::internal::list_tests() -> void
{
    for (auto const& [name, _] : test_container()) {
        std::println("{}", name);
    }
}

auto cppunittest::internal::run_test(std::string_view const requested_name) -> void
{
    for (auto const& [test_name, test_runner] : test_container()) {
        if (test_name == requested_name) {
            test_runner();
            return;
        }
    }
    throw std::runtime_error { std::format("No test named '{}' found", requested_name) };
}

cppunittest::internal::Test_adder::Test_adder(
    std::string_view const name, void (*const test_runner)()) noexcept
{
    test_container().emplace_back(name, test_runner);
}
