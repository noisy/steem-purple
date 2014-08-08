set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_library("${PACKAGE_NAME}-obj" OBJECT
    mrim.c
    package.c
    statuses.c
    cl.c
    ft.c
    message.c
    mrim-util.c)

add_library("${PACKAGE_NAME}" SHARED
    $<TARGET_OBJECTS:${PACKAGE_NAME}-obj>)

target_link_libraries("${PACKAGE_NAME}"
    ${REQ_LIBRARIES}
    ${GTK2_LIBRARIES})

if(UNIX)
    install(FILES "${CMAKE_BINARY_DIR}/lib${PACKAGE_NAME}.so" DESTINATION "${LIBRARY_DIRECTORY}/purple-2/")
endif()
