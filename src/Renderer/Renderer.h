#pragma once

#include <string>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

namespace Renderer {
	class Renderer {
	public:
		static void Draw( 
			const VertexArray& 		vertexArray,
			const IndexBuffer& 		indexBuffer,
			const ShaderProgram& 	shaderProgram
		);

		static void setClearColor(
			float 	red,
			float 	green,
			float 	blue,
			float 	alpha
		);

		static void clear();	
		static void setViewport(
			unsigned int width,
			unsigned int height,
			unsigned int offsetX = 0,
			unsigned int offsetY = 0
		);

		static std::string getRendererName() noexcept { 
			return (char*)glGetString(GL_RENDERER);
		}; 

		static std::string getOpenGLVersion() noexcept { 
			return (char*)glGetString(GL_VERSION);
		}; 

	private:


	};
} // namespace Renderer
