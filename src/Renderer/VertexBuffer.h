#pragma once

#include <glad/gl.h>

namespace Renderer {
	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) 				= delete;
		VertexBuffer& operator=(const VertexBuffer&) 	= delete;

		VertexBuffer(VertexBuffer&&)					noexcept;
		VertexBuffer& operator=(VertexBuffer&&)			noexcept;

		void init(const void* data, 	const size_t dataSize);
		void update(const void* data, 	const size_t dataSize) 	const;
		void bind() 											const;
		void unbind() 											const;
	private:
		GLuint m_id;
	};
} // namespace Renderer

