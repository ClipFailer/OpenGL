#include "VertexBuffer.h"

namespace Renderer {
	VertexBuffer::VertexBuffer()
		: m_id(0)
	{

	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs) noexcept {
		m_id = rhs.m_id;

		rhs.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs) noexcept {
		m_id = rhs.m_id;

		rhs.m_id = 0;

		return *this;
	}

	void VertexBuffer::init(const void* data, const size_t dataSize) {
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::update(const void* data, const size_t dataSize) const {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}