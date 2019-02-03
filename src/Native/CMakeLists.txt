PROJECT(Revery_Native)

CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

ADD_EXECUTABLE(Revery_Native dialog_gtk.c)

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(Revery_Native ${GTK3_LIBRARIES})
