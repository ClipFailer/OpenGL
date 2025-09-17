#include "VertexArray.h"

namespace Renderer {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& rhs) noexcept
		: m_id(rhs.m_id) 
	{
		rhs.m_id = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& rhs) noexcept {
		if (this != &rhs) {
			glDeleteVertexArrays(1, &m_id);
			m_id = rhs.m_id;
			rhs.m_id = 0;
		}

		return *this;
	}

	void VertexArray::addBuffer(
		const VertexBuffer 		&vBuffer, 
		const VertexArrayLayout &layout) const 
	{
		bind();
		vBuffer.bind();

		const auto& elements = layout.getElements();
		size_t stride = layout.getStride();

		for (const auto& element : elements) {
			glEnableVertexAttribArray(m_attribIndex);
			glVertexAttribPointer(
				m_attribIndex,
				element.count,
				element.type,
				element.normalized,
				stride,
				(const void*)element.offset
			);

			++m_attribIndex;
		}
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const {
	glBindVertexArray(0);
	}

    GLuint VertexArray::getId() const {
        return m_id;
    }
} // namespace Renderer