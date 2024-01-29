#pragma once

#include <concepts>
#include <format>
#include <source_location>
#include <string_view>

namespace cppunittest::internal {
    enum class Assertion_type { require, check };

    auto perform_assert(
        Assertion_type       type,
        bool                 value,
        std::string_view     description,
        std::source_location caller = std::source_location::current()) -> void;

    auto perform_type_erased_assert_equals(
        Assertion_type       type,
        bool                 equal,
        std::string_view     description,
        std::format_args     format_args,
        std::source_location caller) -> void;

    template <std::formattable<char> L, std::formattable<char> R>
        requires std::equality_comparable_with<L, R>
    auto perform_assert_equals(
        Assertion_type       type,
        L const&             lhs,
        R const&             rhs,
        std::string_view     description,
        std::source_location caller = std::source_location::current()) -> void
    {
        ::cppunittest::internal::perform_type_erased_assert_equals(
            type, lhs == rhs, description, std::make_format_args(lhs, rhs), caller);
    }

    auto list_tests() -> void;

    [[nodiscard]] auto run_test(std::string_view name) -> int;

    struct Test_adder {
        explicit Test_adder(std::string_view, void (*)());
    };
} // namespace cppunittest::internal

#define CPPUNITTEST_INTERNAL_CONCAT(a, b) a##b

#define CPPUNITTEST_INTERNAL_TEST_IMPL(name, line)                                                 \
    static auto CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line)()->void;         \
    [[maybe_unused]] static ::cppunittest::internal::Test_adder const CPPUNITTEST_INTERNAL_CONCAT( \
        cppunittest_internal_test_adder_, line) {                                                  \
        name,                                                                                      \
        CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line),                         \
    };                                                                                             \
    static auto CPPUNITTEST_INTERNAL_CONCAT(cppunittest_internal_run_test_, line)() -> void

#define CPPUNITTEST_INTERNAL_ASSERT(name, type, ...)   \
    ::cppunittest::internal::perform_assert(           \
        ::cppunittest::internal::Assertion_type::type, \
        static_cast<bool>(__VA_ARGS__),                \
        name "(" #__VA_ARGS__ ")")

#define CPPUNITTEST_INTERNAL_ASSERT_THROWS_AS(exception, name, type, ...) \
    ::cppunittest::internal::perform_assert(                              \
        ::cppunittest::internal::Assertion_type::type,                    \
        [&] {                                                             \
            try {                                                         \
                static_cast<void>(__VA_ARGS__);                           \
                return false;                                             \
            }                                                             \
            catch (exception const&) {                                    \
                return true;                                              \
            }                                                             \
        }(),                                                              \
        name "(" #exception ", " #__VA_ARGS__ ")")

#define CPPUNITTEST_INTERNAL_ASSERT_EQUALS(name, type, ...) \
    ::cppunittest::internal::perform_assert_equals(         \
        ::cppunittest::internal::Assertion_type::type, __VA_ARGS__, name "(" #__VA_ARGS__ ")")

#define REQUIRE(...) CPPUNITTEST_INTERNAL_ASSERT("REQUIRE", require, __VA_ARGS__)
#define CHECK(...) CPPUNITTEST_INTERNAL_ASSERT("CHECK", check, __VA_ARGS__)

#define REQUIRE_THROWS_AS(exception, ...) \
    CPPUNITTEST_INTERNAL_ASSERT_THROWS_AS(exception, "REQUIRE_THROWS_AS", require, __VA_ARGS__)
#define CHECK_THROWS_AS(exception, ...) \
    CPPUNITTEST_INTERNAL_ASSERT_THROWS_AS(exception, "CHECK_THROWS_AS", check, __VA_ARGS__)

#define REQUIRE_EQUAL(...) CPPUNITTEST_INTERNAL_ASSERT_EQUALS("REQUIRE_EQUAL", require, __VA_ARGS__)
#define CHECK_EQUAL(...) CPPUNITTEST_INTERNAL_ASSERT_EQUALS("CHECK_EQUAL", check, __VA_ARGS__)

#define UNITTEST(name) CPPUNITTEST_INTERNAL_TEST_IMPL(name, __LINE__) // NOLINT
