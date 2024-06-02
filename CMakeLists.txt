cmake_minimum_required(VERSION  3.10)
project(C_STRING)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directory
include_directories(include)

# Add the subdirectory (your C library)
# If the TARGET is included any other way for example by another library
# in the same project than we will not include it twice.
if(NOT TARGET ARENA_ALLOCATOR)
    add_subdirectory(libs/arena_allocator)
endif()

target_include_directories(ARENA_ALLOCATOR INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/libs/arena_allocator/include)

# Library source files
add_library(C_STRING STATIC src/c_string.c)  # or SHARED for a shared library
target_link_libraries(C_STRING PRIVATE ARENA_ALLOCATOR)

# Set target properties (optional but recommended)
set_target_properties(C_STRING PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    PUBLIC_HEADER include/arena.h
)

# Install the library and header file
install(TARGETS C_STRING
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)