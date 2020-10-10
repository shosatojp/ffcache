cmake_minimum_required(VERSION 3.16)
project(ffcache)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_FIND_DEBUG_MODE 1)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(PYTHON python3)
# execute_process(COMMAND ${PYTHON} -m pybind11 --include 
#                 OUTPUT_VARIABLE PYBIND11_INCLUDE_DIR 
#                 OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND ${PYTHON}-config --includes
#                 OUTPUT_VARIABLE PYTHON_INCLUDE_DIR 
#                 OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${PYTHON}-config --extension-suffix
                OUTPUT_VARIABLE PYTHON_EXT_SUFFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# include_directories(
#     ${PYBIND11_INCLUDE_DIR}
#     ${PYTHON_INCLUDE_DIR})

# add_executable(ffcache-cli
#                 src/util.cpp
#                 src/httpheader.cpp
#                 src/ffcacheentry.cpp
#                 src/structs.cpp
#                 src/ffcacheindex.cpp
#                 src/main.cpp
#                 src/ffcache.cpp)
add_library(ffcache SHARED
                src/util.cpp
                src/httpheader.cpp
                src/ffcacheentry.cpp
                src/structs.cpp
                src/ffcacheindex.cpp
                src/pymain.cpp
                src/ffcache.cpp)
set_target_properties(ffcache PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "_ffcache${PYTHON_EXT_SUFFIX}"
    SUFFIX "")