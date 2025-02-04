#include <cppunittest/unittest.hpp>

#include <cstdlib>
#include <print>
#include <span>

namespace {
    void show_usage()
    {
        std::println(
            stderr,
            "Usage:\n"
            "\t-h, --help   : Show this help text\n"
            "\t--list-tests : Show registered tests\n"
            "\tabc xyz      : Run the tests named abc and xyz");
    }

    void handle_flag(std::string_view const flag)
    {
        if (flag == "-h" || flag == "--help") {
            show_usage();
        }
        else if (flag == "--list-tests") {
            cppunittest::internal::list_tests();
        }
        else {
            std::println(stderr, "Invalid flag: '{}'", flag);
            show_usage();
            std::quick_exit(EXIT_FAILURE);
        }
    }

    auto run_test(std::string_view const name) -> int
    {
        try {
            return cppunittest::internal::run_test(name);
        }
        catch (std::exception const& exception) {
            std::println(stderr, "Error: {}", exception.what());
            return EXIT_FAILURE;
        }
    }
} // namespace

auto main(int const argc, char const* const* const argv) -> int
{
    bool interpret_flags = true;
    int  exit_status     = EXIT_SUCCESS;

    for (std::string_view argument : std::span(argv + 1, argv + argc)) {
        if (argument == "--") {
            interpret_flags = false;
        }
        else if (interpret_flags and argument.starts_with('-')) {
            handle_flag(argument);
        }
        else {
            int status  = run_test(argument);
            exit_status = (exit_status == EXIT_SUCCESS) ? status : exit_status;
        }
    }

    return exit_status;
}
