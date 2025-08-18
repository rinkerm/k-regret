cmake_minimum_required(VERSION 3.14)

project(k-regret-tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Source Files / Headers
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

#Add Core source file libraries.
add_library(core STATIC ${SOURCES} ${HEADERS})

#Public Include
target_include_directories(core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

#Compiler-/config-specific options
if (MSVC)
  target_compile_options(core PRIVATE /W4 /permissive-)
  target_compile_options(core PRIVATE $<$<CONFIG:Release>:/O2>)
else()
  target_compile_options(core PRIVATE -Wall -Wextra)
  target_compile_options(core PRIVATE $<$<CONFIG:Release>:-O3>)
endif()

#Executable
add_executable(k-regret-tool "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(k-regret-tool PRIVATE core)

set_target_properties(core PROPERTIES FOLDER "Libraries")
set_target_properties(k-regret-tool PROPERTIES FOLDER "Apps")

set(ALL_FILES
  ${SOURCES}
  ${HEADERS}
  "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALL_FILES})