cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VERSION})

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
	message(SEND_ERROR "Failed to find boost")
	return()
endif()

find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

configure_file(version.h.in version.h)

#Main target

add_executable(bulk_server
	main.cpp
	bulk_server.cpp
	client_session.cpp
	async.cpp
	thread_writers/thread_console_writer.cpp
	thread_writers/thread_file_writer.cpp
	collector.cpp
	console.cpp
	context_collector.cpp
	file_logger.cpp
	interpreter.cpp
)

add_library(lib_bulk_server lib.cpp)

set_target_properties(
    bulk_server
    lib_bulk_server
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(lib_bulk_server 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk_server PRIVATE
    ${Boost_LIBRARIES}
    lib_bulk_server
)

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(lib_bulk_server PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(lib_bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bulk_server RUNTIME DESTINATION bin)
install(TARGETS lib_bulk_server RUNTIME DESTINATION lib)
#install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()