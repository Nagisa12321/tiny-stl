cmake_minimum_required(VERSION 3.10)

project(Tiny-STL)
message(STATUS "Start Cmake...")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# the .h files
add_library(tiny_stl_interface INTERFACE)
target_include_directories(tiny_stl_interface INTERFACE include)

# project files
set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_TESTS ${PROJECT_SOURCE_DIR}/test)
FILE(GLOB TINY_STL_SOURCE_FILES ${PROJECT_SOURCES}/*.cpp)
FILE(GLOB TINY_STL_TEST_FILES ${PROJECT_TESTS}/*.c*)

# show the file found
message(" > source files: ")
foreach(X ${TINY_STL_SOURCE_FILES})
    get_filename_component(raw_name ${X} NAME)
    message("${raw_name}")
endforeach()
message(" > test files: ")
foreach(X ${TINY_STL_TEST_FILES})
    get_filename_component(raw_name ${X} NAME)
    message("${raw_name}")
endforeach()
message(" > end")

# tmp object
add_library(tiny_stl_sources_obj OBJECT ${TINY_STL_SOURCE_FILES})
target_link_libraries(tiny_stl_sources_obj PRIVATE tiny_stl_interface)

# create the test files
foreach(X ${TINY_STL_TEST_FILES})
    get_filename_component(raw_name ${X} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" cooked_name ${raw_name})

    message(STATUS "make and link the test file \"${cooked_name}\"")
    add_executable(${cooked_name} ${X} $<TARGET_OBJECTS:tiny_stl_sources_obj>)
    target_link_libraries(${cooked_name} PRIVATE tiny_stl_interface)
endforeach()