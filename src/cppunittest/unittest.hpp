#pragma once

#include <concepts>
#include <format>
#include <source_location>
#include <string_view>

#define CPPUNITTEST_INTERNAL_CONCAT(a, b) a##b

#define CPPUNITTEST_INTERNAL_TEST_IMPL(name, line)                                                 \
    static auto CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line)()->void;         \
    [[maybe_unused]] static ::cppunittest::internal::Test_adder const CPPUNITTEST_INTERNAL_CONCAT( \
        cppunittest_internal_test_adder_, line) {                                                  \
        name,                                                                                      \
        CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line),                         \
    };                                                                                             \
    static auto CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line)() -> void

#define REQUIRE(...) \
    ::cppunittest::internal::require(static_cast<bool>(__VA_ARGS__), "REQUIRE(" #__VA_ARGS__ ")")

#define REQUIRE_EQUAL(...) \
    ::cppunittest::internal::require_equal(__VA_ARGS__, "REQUIRE_EQUAL(" #__VA_ARGS__ ")")

#define UNITTEST(name) CPPUNITTEST_INTERNAL_TEST_IMPL(name, __LINE__) // NOLINT

namespace cppunittest::internal {

    auto require(
        bool                 value,
        std::string_view     description,
        std::source_location caller = std::source_location::current()) -> void;

    auto do_require_equal(
        bool                 equal,
        std::string_view     description,
        std::format_args     format_args,
        std::source_location caller) -> void;

    template <std::formattable<char> L, std::formattable<char> R>
        requires std::equality_comparable_with<L, R>
    auto require_equal(
        L const&             lhs,
        R const&             rhs,
        std::string_view     description,
        std::source_location caller = std::source_location::current()) -> void
    {
        ::cppunittest::internal::do_require_equal(
            lhs == rhs, description, std::make_format_args(lhs, rhs), caller);
    }

    auto list_tests() -> void;

    auto run_test(std::string_view name) -> void;

    struct Test_adder {
        explicit Test_adder(std::string_view, void (*)()) noexcept;
    };

} // namespace cppunittest::internal
