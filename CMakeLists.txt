cmake_minimum_required(VERSION 3.21)

project(RTMidi)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
find_package(RTMIDI REQUIRED)

function(set_high_warnings_level target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    else ()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif ()
endfunction()

set(CMAKE_CXX_STANDARD 20)

if (PROJECT_IS_TOP_LEVEL)

    set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version" FORCE)

    option(UniversalBinary "Build universal binary for mac" OFF)

    if (UniversalBinary)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
    endif ()

    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

add_subdirectory(RTMIDI)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(Examples)
endif ()
