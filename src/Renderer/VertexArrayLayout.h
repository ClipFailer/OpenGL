#pragma once

#include <vector>

#include <glad/gl.h>

namespace Renderer {

	struct VertexArrayLayoutElement {
		GLuint count;
		GLenum type;
		GLboolean normalized;
		size_t offset;
	};

	class VertexArrayLayout {
	public:
		VertexArrayLayout(size_t stride = 0);

		unsigned int getStride() const noexcept { return m_stride; };

		const std::vector<VertexArrayLayoutElement>& 
		getElements() const noexcept { return m_layoutElements; };

		void reserveElements(const size_t count);

		template <typename T>
		void addElement(
			GLuint 		count, 
			size_t 		offset, 
			GLboolean 	normalized = GL_FALSE
		);

	private:
		std::vector<VertexArrayLayoutElement> m_layoutElements;
		unsigned int m_stride;

	};
} // namespace Renderer
