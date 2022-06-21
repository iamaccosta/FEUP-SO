cmake_minimum_required(VERSION 3.15)
project(SO_proj C)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_options INTERFACE)

# standard compiler warnings
set(CLANG_WARNINGS
        -Wall
        -Wextra # reasonable and standard
        -Wshadow # warn the user if a variable declaration shadows one from a parent context
        -Wcast-align # warn for potential performance problem casts
        -Wunused # warn on anything being unused
        -Wpedantic # warn if non-standard C++ is used
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wnull-dereference # warn if a null dereference is detected
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output (ie printf)
        )

set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
        )

if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(project_options INTERFACE ${CLANG_WARNINGS})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(project_options INTERFACE ${GCC_WARNINGS})
endif()

add_executable(phrases phrases.c)
target_link_libraries(phrases PRIVATE project_options)

add_executable(addmx addmx.c)
target_link_libraries(addmx PRIVATE project_options)

add_executable(cypher cypher.c)
target_link_libraries(cypher PRIVATE project_options Threads::Threads)
