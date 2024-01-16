# Script parameters:
# - EXECUTABLE:    The test executable
# - CTEST_INCLUDE: The file to be included by CTest

if (NOT EXISTS "${EXECUTABLE}")
    message(FATAL_ERROR "Executable '${EXECUTABLE}' does not exist")
endif ()

if (${CTEST_INCLUDE} STREQUAL "")
    message(FATAL_ERROR "No CTest include path provided")
endif ()

# Get the list of tests from the executable
execute_process(
    COMMAND "${EXECUTABLE}" --list-tests
    OUTPUT_VARIABLE output
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "Could not retrieve tests from ${EXECUTABLE}")
endif ()

# Prepare the list for iteration
string(REPLACE "\n" ";" output "${output}")

# Accumulator for the `add_test` calls
set(script)

# Register each of the discovered tests
foreach (line IN LISTS output)
    if (NOT "${line}" STREQUAL "")
        set(script "${script}add_test(\"${line}\" \"${EXECUTABLE}\" --run-test \"${line}\")\n")
    endif ()
endforeach ()

# Write the script to the CTest include file
file(WRITE "${CTEST_INCLUDE}" "${script}")
