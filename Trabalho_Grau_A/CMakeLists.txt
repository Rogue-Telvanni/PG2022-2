cmake_minimum_required(VERSION 3.2)
project(main)


#GLFW additions
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies)
include_directories(dependencies/GLFW3/include)
include_directories(dependencies/GLFW3/lib)
include_directories(dependencies/GLFW3)
include_directories(dependencies/GLAD/include)
include_directories(dependencies/glm)

set(GLAD "${CMAKE_SOURCE_DIR}/dependencies/GLAD/include/glad/glad.h"
         "${CMAKE_SOURCE_DIR}/dependencies/GLAD/src/glad.c")

set(SHADER "${CMAKE_SOURCE_DIR}/HelloTriangle/Shader.h"
           "${CMAKE_SOURCE_DIR}/HelloTriangle/Shader.cpp")

set(IMAGE "${CMAKE_SOURCE_DIR}/HelloTriangle/stb_image.h"
           "${CMAKE_SOURCE_DIR}/HelloTriangle/stb_image.cpp")
set(TIMER "${CMAKE_SOURCE_DIR}/HelloTriangle/Timer.h"
          "${CMAKE_SOURCE_DIR}/HelloTriangle/Timer.cpp")

find_package(OpenGL REQUIRED)
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/HelloTriangle/Source.cpp)

add_executable(main ${SOURCE_FILES} ${GLAD} ${SHADER} ${IMAGE} ${TIMER})

# created for xorg X11 need to study to make it work on wayland
set(LIBS ${CMAKE_SOURCE_DIR}/dependencies/GLFW3/lib/libglfw3.a rt m dl X11 pthread xcb Xau)
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

target_link_libraries(main ${LIBS})