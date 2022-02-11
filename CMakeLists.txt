cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set macro-directory and find scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# include cmake custom functions
set(WITH_SOURCE_TREE ON)
include(GroupSources)
include(AutoCollect)
include(MakeLibrary)

# set the project name
project(http-gui)

add_subdirectory(src)

option(ENABLE_HTTP_GUI_TESTS "Enable tests generation" ON)
if(ENABLE_HTTP_GUI_TESTS)
    add_subdirectory(tests)
endif()

add_executable(DummyServer DummyServer.cpp)
target_link_libraries(DummyServer HttpGui)

