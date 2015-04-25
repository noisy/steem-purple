set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_library("${PACKAGE_NAME}.o" OBJECT
    mrim.c
    package.c
    statuses.c
    cl.c
    message.c
    mrim-util.c)

# Only link ft.c is file transfer support is enabled.
if(ENABLE_FILES)
    add_library("${PACKAGE_NAME}.o" OBJECT ft.c)
endif()

add_library("${PACKAGE_NAME}" SHARED
    $<TARGET_OBJECTS:${PACKAGE_NAME}.o>)

target_link_libraries("${PACKAGE_NAME}"
    ${REQ_LIBRARIES}
    ${GTK2_LIBRARIES})

if(UNIX)
    install(FILES "${CMAKE_BINARY_DIR}/lib${PACKAGE_NAME}.so" DESTINATION "${LIB_INSTALL_DIR}/purple-2/")
endif()
