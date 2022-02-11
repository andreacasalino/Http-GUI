set(PROJECT_SHORTNAME "HttpGui")

MakeLibrary(${PROJECT_SHORTNAME} header)

target_include_directories(${PROJECT_SHORTNAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/header>
)

find_package(Threads)
target_link_libraries(${PROJECT_SHORTNAME} PRIVATE
	${CMAKE_THREAD_LIBS_INIT}
)

include(FetchContent)
FetchContent_Declare(
	json_lib
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG        v3.10.5
)
FetchContent_MakeAvailable(json_lib)
target_link_libraries(${PROJECT_SHORTNAME} PUBLIC
	nlohmann_json::nlohmann_json
)

SET(BUILD_COLORED_STREAM_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
	colored_stream
	GIT_REPOSITORY https://github.com/andreacasalino/ColoredStream.git
	GIT_TAG        4c7fcf010c73833473cfc4411299504108e00c52
)
FetchContent_MakeAvailable(colored_stream)
target_link_libraries(${PROJECT_SHORTNAME} PRIVATE
    ColoredStream
)
