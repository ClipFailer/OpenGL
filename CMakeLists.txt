cmake_minimum_required(VERSION 4.1 FATAL_ERROR)
project(OpenGLLearning)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
	src/Tools/Tools.h
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)