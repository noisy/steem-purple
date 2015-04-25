enable_testing()

set(LIBCHECK_LIBRARIES "${CHECK_LIBRARIES}")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(UNITTEST_NAME "${PACKAGE_NAME}-testrunner")

add_custom_target("check" COMMAND "${CMAKE_CTEST_COMMAND}")

add_executable("${UNITTEST_NAME}" EXCLUDE_FROM_ALL
    tests.c
    mrim-util-test.c
    $<TARGET_OBJECTS:mrim.o>)

# Can't use ${CHECK_LIBRARIES}, cmake thinks that target "check" is being linked.
target_link_libraries("${UNITTEST_NAME}"
    ${REQ_LIBRARIES}
    ${GTK2_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBCHECK_LIBRARIES})

add_test(NAME "${UNITTEST_NAME}" COMMAND "${UNITTEST_NAME}")
add_dependencies("check" "${UNITTEST_NAME}")
