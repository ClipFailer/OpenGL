cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(OpenGLLearning)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/Texture2D.cpp
	src/Tools/Tools.h
	src/Resources/ResourceManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

include_directories(external/glm)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_target(copy_res ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)