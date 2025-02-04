# Script parameters:
# - EXECUTABLE:    Path to the test executable
# - CTEST_INCLUDE: Path to the CMake script file to be included by CTest
# - WORKING_DIR:   Path to the directory from which the test will be run

if (NOT EXISTS "${EXECUTABLE}")
    message(FATAL_ERROR "Executable '${EXECUTABLE}' does not exist")
endif ()
if (NOT CTEST_INCLUDE)
    message(FATAL_ERROR "No CTest include path provided")
endif ()
if (NOT WORKING_DIR)
    message(FATAL_ERROR "No working directory provided")
endif ()

# Get a newline-separated list of tests from the executable.
execute_process(
    COMMAND "${EXECUTABLE}" --list-tests
    OUTPUT_VARIABLE output
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${WORKING_DIR}")

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "Could not retrieve tests from ${EXECUTABLE}")
endif ()

# Prepare the list for iteration.
string(REPLACE "\n" ";" output "${output}")

# Accumulator for the CTest include script content.
set(script)

# Register the discovered tests.
foreach (test_name IN LISTS output)
    if (NOT "${test_name}" STREQUAL "")
        set(script "${script}add_test(\"${test_name}\" \"${EXECUTABLE}\" \"${test_name}\")\n")
        set(script "${script}set_tests_properties(\"${test_name}\" PROPERTIES WORKING_DIRECTORY \"${WORKING_DIR}\")\n")
    endif ()
endforeach ()

# Write the generated script to the CTest include file.
file(WRITE "${CTEST_INCLUDE}" "${script}")
