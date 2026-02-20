cmake_minimum_required(VERSION 3.21)
project(PollywigEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

find_package(Vulkan REQUIRED)

# Find Vulkan Shaderc
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

# Zlib (for Assimp/minizip)
find_package(ZLIB REQUIRED)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)


# =================================
# Minizip (for Assimp)
# =================================
# Try to locate Minizip headers cross-platform
if(NOT MINIZIP_INCLUDE_DIR)
    if(WIN32)
        # Example Windows paths (adjust if using vcpkg/conan)
        find_path(MINIZIP_INCLUDE_DIR unzip.h
            PATHS
                "C:/Program Files/minizip/include"
                "C:/Program Files (x86)/minizip/include"
        )
    elseif(APPLE)
        # macOS default (Homebrew)
        find_path(MINIZIP_INCLUDE_DIR unzip.h
            PATHS /usr/local/include /opt/homebrew/include
        )
    else()
        # Linux default locations
        find_path(MINIZIP_INCLUDE_DIR unzip.h
            PATHS /usr/include /usr/include/minizip /usr/local/include
        )
    endif()
endif()

if(NOT MINIZIP_INCLUDE_DIR)
    message(WARNING "Minizip headers not found. Assimp may fail to build importers that need minizip.")
else()
    message(STATUS "Using Minizip headers from: ${MINIZIP_INCLUDE_DIR}")
    include_directories(SYSTEM ${MINIZIP_INCLUDE_DIR})
endif()

# =================================
# Assimp
# =================================
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

# GLM
add_subdirectory(external/glm)

# vk-bootstrap
add_subdirectory(external/vk-bootstrap)

# Jolt-Physics
add_subdirectory(external/JoltPhysics)

add_executable(PollywigEngine
    src/main.cpp
    src/VulkanRenderer.cpp
    src/vma_impl.cpp
)

target_include_directories(PollywigEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(PollywigEngine SYSTEM PRIVATE
    external/VMA/include
)

# Include system libraries
target_include_directories(PollywigEngine SYSTEM PRIVATE ${ZLIB_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND
    CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")

    target_compile_options(PollywigEngine PRIVATE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wnon-virtual-dtor
        -Wold-style-cast -Wconversion
        -Wsign-conversion -Wnull-dereference
        -fsanitize=address,undefined
    )

    target_link_options(PollywigEngine PRIVATE
        -fsanitize=address,undefined
    )
endif()

target_link_libraries(PollywigEngine PRIVATE
    Vulkan::Vulkan
    Vulkan::shaderc_combined
    glfw
    assimp
    glm::glm
    vk-bootstrap
    Jolt
    ${ZLIB_LIBRARIES}
)
