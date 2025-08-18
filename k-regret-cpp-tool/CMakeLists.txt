cmake_minimum_required(VERSION 3.14)

project(k-regret-tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Source Files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

#Include directories
include_directories(include)

#Add Core source file libraries.
add_library(core STATIC ${SOURCES})

#Compiler-/config-specific options
if (MSVC)
  #Warnings and strictness
  target_compile_options(core PRIVATE /W4 /permissive-)
  #Optimize Release builds
  target_compile_options(core PRIVATE $<$<CONFIG:Release>:/O2>)
else()
  target_compile_options(core PRIVATE -Wall -Wextra)
  target_compile_options(core PRIVATE $<$<CONFIG:Release>:-O3>)
endif()

#Executable
add_executable(k-regret-tool main.cpp)

target_link_libraries(k-regret-tool PRIVATE core)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)