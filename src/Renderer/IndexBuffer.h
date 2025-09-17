#pragma once

#include <glad/gl.h>

namespace Renderer {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) 			= delete;
		IndexBuffer& operator=(const IndexBuffer&) 	= delete;

		IndexBuffer(IndexBuffer&&);
		IndexBuffer& operator=(IndexBuffer&&);

		void init(const void* data, const size_t count);
		void update(const void* data, const size_t count);
		void bind() const noexcept;
		void unbind() const noexcept;

		size_t getCount() const noexcept { return m_count; };

	private:
		GLuint m_id;
		size_t m_count;
	};
}