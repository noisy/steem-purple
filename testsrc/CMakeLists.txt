enable_testing()
add_custom_target("check" COMMAND "${CMAKE_CTEST_COMMAND}")

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(UNITTEST_NAME "${PACKAGE_NAME}-testrunner")
configure_file("config.h.in" "config.h" @ONLY)

add_executable("${UNITTEST_NAME}" EXCLUDE_FROM_ALL
    tests.c
    mrim-util-test.c
    $<TARGET_OBJECTS:mrim-obj>)

target_link_libraries("${UNITTEST_NAME}"
    dl
    ${REQ_LIBRARIES}
    libcheck.so)

add_test(NAME "${UNITTEST_NAME}" COMMAND "${UNITTEST_NAME}")
add_dependencies("check" "${UNITTEST_NAME}")
