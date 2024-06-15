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

set(ASYNC_LIB_SRC
    "async.cpp"
    "async.h"
    "concurrent/concurrent_queue.h"
    "Interfaces/interface_writer.h"
    "thread_writers/thread_console_writer.cpp"
    "thread_writers/thread_console_writer.h"
    "thread_writers/thread_file_writer.cpp"
    "thread_writers/thread_file_writer.h"
    "collector.cpp"
    "collector.h"
    "console.cpp"
    "console.h"
    "context_collector.cpp"
    "context_collector.h"
    "file_logger.cpp"
    "file_logger.h"
    "interpreter.cpp"
    "interpreter.h"
    "threads_joiner.h"
)

#Main target
add_library(async_static STATIC ${ASYNC_LIB_SRC})
add_library(async SHARED ${ASYNC_LIB_SRC})


add_executable(async_cli main.cpp)

set_target_properties(
    async
    async_static
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(async_static ${Boost_LIBRARIES} Threads::Threads)

target_link_libraries(async ${Boost_LIBRARIES} Threads::Threads)
 
target_link_libraries(async_cli PRIVATE async_static)

if (MSVC)
    target_compile_options(async_static PRIVATE
        /W4
    )
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(async_cli PRIVATE
        /W4
    )
else ()
    target_compile_options(async_static PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(async_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS async_static RUNTIME DESTINATION lib)
install(TARGETS async RUNTIME DESTINATION lib)
install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()