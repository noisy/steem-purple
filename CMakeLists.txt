project(mrim-prpl)
cmake_minimum_required(VERSION 2.8.8)
include(FindPkgConfig)
include(FindGettext)
include(CheckIncludeFiles)

set(PACKAGE_NAME "mrim")
set(PROJECT_VERSION "0.2.0")
set(BUILD_NUMBER "null") # TODO: add git revision
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Known working CFLAGS.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -DPIC -std=c99")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code --pedantic -Wstrict-aliasing")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb")

# Default build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Check for header files.
check_include_files("sys/socket.h" HAVE_SYS_SOCKET_H)

pkg_check_modules(REQ "purple>=2.7.0" REQUIRED)
pkg_check_modules(REQ "glib-2.0" REQUIRED)
pkg_check_modules(CHECK "check>=0.9.4" REQUIRED)

option(ENABLE_GTK "Enable gtk+ dialogs" OFF)
option(ENABLE_FILES "Support file transfer" OFF)

if(ENABLE_GTK OR ENABLE_FILES)
    pkg_check_modules(REQ "gtk+-2.0" REQUIRED)
endif()

# Populate libraries.
include_directories(${REQ_INCLUDE_DIRS} ${CHECK_INCLUDE_DIRS})
add_definitions(${REQ_CFLAGS_OTHER} ${CHECK_CFLAGS_OTHER})
link_directories(${REQ_LIBRARY_DIRS} ${CHECK_LIBRARY_DIRS})

# Set constants for UNIX-like.
if(UNIX)
    if(NOT DEFINED LIBRARY_DIRECTORY)
        set(LIBRARY_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
    endif()
    if(NOT DEFINED LANGUAGE_SUPPORT_DIRECTORY)
        set(LANGUAGE_SUPPORT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/locale)
    endif()
    if(NOT DEFINED PIDGIN_PROTOCOLS_DIRECTORY)
        set(PIDGIN_PROTOCOLS_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/pixmaps/pidgin/protocols)
    endif()
endif()

# Add subdirectories.
add_subdirectory(src)
add_subdirectory(pixmaps)
add_subdirectory(testsrc)

if(GETTEXT_FOUND)
    add_subdirectory(po)
else()
    message("gettext is not found, translation support is disabled!")
endif()
