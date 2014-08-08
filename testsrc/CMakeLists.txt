enable_testing()
add_custom_target("check" COMMAND "${CMAKE_CTEST_COMMAND}")

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(UNITTEST_NAME "${PACKAGE_NAME}-testrunner")

add_executable("${UNITTEST_NAME}" EXCLUDE_FROM_ALL
    tests.c
    mrim-util-test.c
    $<TARGET_OBJECTS:mrim-obj>)

add_test(NAME "${UNITTEST_NAME}" COMMAND "${UNITTEST_NAME}")
add_dependencies("check" "${UNITTEST_NAME}")
