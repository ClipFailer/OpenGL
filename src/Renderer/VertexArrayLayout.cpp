#include "VertexArrayLayout.h"

namespace Renderer {
	VertexArrayLayout::VertexArrayLayout(size_t stride) 
		: m_stride(stride)
	{

	};

	void VertexArrayLayout::reserveElements(const size_t count) {
		m_layoutElements.reserve(count);
	};

	template<>
	void VertexArrayLayout::addElement<float>(
		GLuint 		count, 
		size_t 		offset, 
		GLboolean 	normalized
	) {
		m_layoutElements.push_back({count, GL_FLOAT, normalized, offset});
	}
} // namespace Renderer
