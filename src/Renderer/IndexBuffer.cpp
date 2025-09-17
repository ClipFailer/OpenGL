#include "IndexBuffer.h"

namespace Renderer {
	IndexBuffer::IndexBuffer()
		: m_id(0)
		, m_count(0)
	{

	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& rhs) {
		m_id = rhs.m_id;
		m_count = rhs.m_count;
		rhs.m_id = 0;
		rhs.m_count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& rhs) {
		m_id = rhs.m_id;
		m_count = rhs.m_count;
		rhs.m_id = 0;
		rhs.m_count = 0;
		return *this;
	}

	void IndexBuffer::init(const void* data, const size_t count) {
		m_count = count;

		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			count * sizeof(GLuint), 
			data, 
			GL_STATIC_DRAW
		);
	}

	void IndexBuffer::update(const void* data, const size_t dataSize) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dataSize, data);
	}

	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
} // namespace Renderer
