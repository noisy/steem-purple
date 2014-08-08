project(mrim-prpl)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(FindPkgConfig)
include(FindGettext)
include(CheckIncludeFiles)
include(GetGitRevisionDescription)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

set(PACKAGE_NAME "mrim")
set(PROJECT_VERSION "0.2.0")
# Set current git revision as BUILD_NUMBER definition.
get_git_head_revision(BUILD_BRANCH BUILD_NUMBER)

# Known working CFLAGS.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -DPIC -std=c99")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code --pedantic -Wstrict-aliasing")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb")

# Default build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Definitions for source code.
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS PROJECT_VERSION="${PROJECT_VERSION}")
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS BUILD_NUMBER="${BUILD_NUMBER}")

# Check for sys/socket.h.
check_include_files("sys/socket.h" HAVE_SYS_SOCKET_H)

pkg_check_modules(REQ
    "purple>=2.7.0"
    "glib-2.0"
    "check>=0.9.4"
    REQUIRED)

# Populate libraries.
include_directories(${REQ_INCLUDE_DIRS})
add_definitions(${REQ_CFLAGS_OTHER})

option(ENABLE_GTK "Enable gtk+ dialogs" ON)
option(ENABLE_FILES "Support file transfer" OFF)

if(ENABLE_GTK)
    pkg_check_modules(GTK2 "gtk+-2.0" REQUIRED)

    # Populate Gtk2.
    include_directories(${GTK2_INCLUDE_DIRS})
    add_definitions(${GTK2_CFLAGS_OTHER})

    # Gtk definition for source code.
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ENABLE_GTK)
endif()

if(ENABLE_FILES)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ENABLE_FILES)
endif()

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
