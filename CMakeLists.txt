cmake_minimum_required(VERSION 3.12)

project(HelloWorldApp)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/cpm.cmake)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Option to enable testing
option(ENABLE_TESTING "Enable testing" ON)

# Option to specify the GTest installation directory
set(GTEST_INSTALL_DIR "" CACHE PATH "Path to Google Test installation directory")

if (ENABLE_TESTING)
    enable_testing()

    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.12.1
        VERSION 1.12.1
        OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
    )

    add_subdirectory(test) # Include the test subdirectory
endif()

# Add an executable for the main application
add_executable(vtable src/main.cpp)

# treat all warnings as errors
if (MSVC)
    target_compile_options(vtable PRIVATE /Wall /WX /wd4668 /wd4711 /wd4710)
else()
    target_compile_options(vtable PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
