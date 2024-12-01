cmake_minimum_required(VERSION 3.9)
project(aoc24 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_BUILD_TYPE Release)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wno-c23-extensions")
endif()

set(executables d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15 d16 d17 d18 d19 d20 d21 d22 d23 d24 d25)

foreach(executable ${executables})
    file(GLOB_RECURSE sources ${executable}/*.cxx ${executable}/*.hxx)
    add_executable(${executable} ${sources}) 
endforeach()

# For brute forcing
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    foreach(executable ${executables})
        target_link_libraries(${executable} PUBLIC OpenMP::OpenMP_CXX)
    endforeach()
endif()

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -checks=*;
    -header-filter=.*;
)
