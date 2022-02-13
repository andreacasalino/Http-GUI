project(Tests)

include(FetchContent)
FetchContent_Declare(
catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
GIT_TAG        4ff8b27bb6bed4b8b309e56cd269b4f1fbc24e89
)
FetchContent_MakeAvailable(catch2)

set(TESTS_NAME Tests)

file(GLOB SOURCES [RELATIVE path] ./*.cpp)

add_executable(${TESTS_NAME} ${SOURCES})

target_link_libraries(${TESTS_NAME} PUBLIC
    Catch2::Catch2
    Catch2::Catch2WithMain
    HttpGui
)

install(TARGETS ${TESTS_NAME})
