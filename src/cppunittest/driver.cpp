#include <cppunittest/unittest.hpp>
#include <cppargs.hpp>

#include <cstdlib>
#include <print>

auto main(int const argc, char const* const* const argv) -> int
{
    cppargs::Parameters parameters;

    auto const help_flag       = parameters.add('h', "help", "Show this help text");
    auto const list_tests_flag = parameters.add("list-tests", "List all registered tests");
    auto const run_test_option = parameters.add<std::string_view>("run-test", "Run the given test");

    try {
        cppargs::parse(argc, argv, parameters);
        if (help_flag) {
            std::print("Valid options:\n{}", parameters.help_string());
        }
        else if (list_tests_flag) {
            cppunittest::internal::list_tests();
        }
        else if (run_test_option) {
            cppunittest::internal::run_test(run_test_option.value());
        }
        return EXIT_SUCCESS;
    }
    catch (cppargs::Exception const& exception) {
        std::println(
            stderr, "Error: {}; use '--help' to see a list of valid options", exception.what());
    }
    catch (std::exception const& exception) {
        std::println(stderr, "Error: {}", exception.what());
    }
    return EXIT_FAILURE;
}
