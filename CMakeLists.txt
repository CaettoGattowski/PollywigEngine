cmake_minimum_required(VERSION 3.21)
project(PollywigEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

find_package(Vulkan REQUIRED)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

# GLM
add_subdirectory(external/glm)

add_executable(PollywigEngine src/main.cpp)

target_include_directories(PollywigEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable warnings and sanitizers for engine only
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(PollywigEngine PRIVATE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wnon-virtual-dtor
        -Wold-style-cast -Wconversion
        -Wsign-conversion -Wnull-dereference
        -fsanitize=address,undefined
    )
    target_link_options(PollywigEngine PRIVATE -fsanitize=address,undefined)
endif()

target_link_libraries(PollywigEngine
    Vulkan::Vulkan
    glfw
    assimp
    glm
)