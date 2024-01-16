include(CTest)

# Set this here so it can be retrieved later from anywhere by `cppunittest_add_tests`
set(CPPUNITTEST_POST_BUILD_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/cppunittest_add_tests.cmake")

function (cppunittest_add_tests executable)
    target_link_libraries("${executable}" PRIVATE cppunittest)
    set(CTEST_INCLUDE "${executable}-cppunittest.cmake")
    add_custom_command(
        TARGET "${executable}" POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
            "-D" "EXECUTABLE=$<TARGET_FILE:${executable}>"
            "-D" "CTEST_INCLUDE=${CTEST_INCLUDE}"
            "-P" "${CPPUNITTEST_POST_BUILD_SCRIPT}"
        VERBATIM)
    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${CTEST_INCLUDE}")
endfunction ()
