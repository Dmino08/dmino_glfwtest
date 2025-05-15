cmake_minimum_required(VERSION 3.30)

project(GLFWTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(CMAKE_FIND_DEBUG_MODE ON)
#set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(external/glad)
add_subdirectory(external/stb)

find_package(OpenGL REQUIRED)

find_package(assimp CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

file(GLOB_RECURSE source "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(GLFWTest ${source})

add_custom_command(TARGET GLFWTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:GLFWTest>/res
)

add_custom_command(TARGET GLFWTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:GLFWTest>
)


target_link_libraries(
    GLFWTest 
    PRIVATE
        OpenGL::GL
        assimp::assimp
        glfw
        glad
    PUBLIC
        glm::glm
        stb
    )




target_include_directories(
    GLFWTest
    PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/include")

configure_file(config.h.in config.h)

