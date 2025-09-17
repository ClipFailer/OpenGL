#include "Renderer.h"

namespace Renderer {
	void Renderer::Draw( 
		const VertexArray& 		vertexArray,
		const IndexBuffer& 		indexBuffer,
		const ShaderProgram& 	shaderProgram
	) {
		shaderProgram.Use();
		vertexArray.bind();
		indexBuffer.bind();

		glDrawElements(
			GL_TRIANGLES, 
			indexBuffer.getCount(),
			GL_UNSIGNED_INT,
			nullptr
		);

	};

	 void Renderer::setClearColor(
		float 	red,
		float 	green,
		float 	blue,
		float 	alpha
	) {
		glClearColor(red, green, blue, alpha);
	};

	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	};

	void Renderer::setViewport(
		unsigned int width,
		unsigned int height,
		unsigned int offsetX,
		unsigned int offsetY
	) {
		glViewport(offsetX, offsetY, width, height);
	};

} // namespace Renderer
