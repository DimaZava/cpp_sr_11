cmake_minimum_required(VERSION 3.5)

project(sr_11)

find_package(Boost 1.83.0 REQUIRED COMPONENTS)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(bayan main.cpp)
add_library(command_info command_info.h command_info.cpp)
add_library(utils utils.h utils.cpp)

target_link_libraries(bayan 
PRIVATE_HEADER
${CMAKE_THREAD_LIBS_INIT}
command_info
utils
)

set_target_properties(bayan command_info utils PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(command_info PRIVATE
        /W4
    )
    target_compile_options(utils PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(command_info PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(utils PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

if(APPLE)
    set(CPACK_GENERATOR "7Z")
else()
    set(CPACK_GENERATOR DEB)
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dimazava@hotmail.com)

include(CPack)