cmake_minimum_required(VERSION 3.5)

set (APPLICATION_NAME allocator)

project(${APPLICATION_NAME} VERSION $ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in version.h)

add_executable(${APPLICATION_NAME} allocator.hpp container.hpp main.cpp)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(${APPLICATION_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(${APPLICATION_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${APPLICATION_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror -O0 -g3
    )
endif()

install(TARGETS ${APPLICATION_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT sacrartorium@example.com)

include(CPack)
