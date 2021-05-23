set(PROJECT_SHORTNAME "Http-GUI")

MakeLibrary(${PROJECT_SHORTNAME} header)

target_include_directories(${PROJECT_SHORTNAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/header>
)

find_package(Threads)
target_link_libraries(${PROJECT_SHORTNAME}
    PRIVATE
	${CMAKE_THREAD_LIBS_INIT}
)
