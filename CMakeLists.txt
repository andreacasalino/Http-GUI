cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# only windows specific stuff
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# set macro-directory and find scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# include cmake custom functions
set(WITH_SOURCE_TREE ON)
include(GroupSources)
include(AutoCollect)
include(Log)
include(SetInstallRPath)

# set the project name
project(http-cpp-server-sample)

add_subdirectory(Lib)

add_executable(DummyServer DummyServer.cpp)
target_link_libraries(DummyServer gui::Http-GUI)

