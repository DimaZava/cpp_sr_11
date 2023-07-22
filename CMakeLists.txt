cmake_minimum_required(VERSION 3.5)

project(sr_11)

find_package(Boost 1.83.0 COMPONENTS thread filesystem system REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

message("boost lib: ${Boost_LIBRARIES}")
message("boost inc:${Boost_INCLUDE_DIR}")
message("boost inc:${Boost_INCLUDE_DIRS}")

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

add_executable(bayan 
    main.cpp 
    utils.cpp
    command_info.cpp
)

target_link_libraries(bayan
    ${Boost_LIBRARIES}
)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
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