cmake_minimum_required(VERSION 4.1 FATAL_ERROR)

project(OpenGLLearning)

add_subdirectory(external/glfw)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
