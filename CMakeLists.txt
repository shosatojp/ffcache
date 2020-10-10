cmake_minimum_required(VERSION 3.16)
project(ffcache)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_FIND_DEBUG_MODE 1)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(PYTHON python3)
execute_process(COMMAND ${PYTHON} -m pybind11 --include 
                OUTPUT_VARIABLE PYBIND11_FLAGS 
                OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${PYTHON}-config --extension-suffix
                OUTPUT_VARIABLE PYTHON_EXT_SUFFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${PYTHON}-config --prefix
                OUTPUT_VARIABLE PYTHON_PREFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${PYBIND11_FLAGS} ${CMAKE_CXX_FLAGS}")


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
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(ffcache PROPERTIES
                          LINK_DIRECTORIES "${PYTHON_PREFIX}/lib")
    file(GLOB PYTHON_LIB "${PYTHON_PREFIX}/lib/libpython3*")
    get_filename_component(PYTHON_LIB ${PYTHON_LIB} NAME)
    message(${PYTHON_LIB})
    target_link_libraries(ffcache ${PYTHON_LIB})
endif()
