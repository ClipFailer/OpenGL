#pragma once

#include <glad/gl.h>

#include "VertexBuffer.h"
#include "VertexArrayLayout.h"

namespace Renderer {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) 				= delete;
		VertexArray& operator=(const VertexArray&) 		= delete;

		VertexArray(VertexArray&&)						noexcept;
		VertexArray& operator=(VertexArray&&)			noexcept;

		void addBuffer(
			const VertexBuffer& 		vBuffer, 
			const VertexArrayLayout& 	layout) 		const;
		void bind() 									const;
		void unbind() 									const;
		GLuint getId() 									const;

	private:
		GLuint m_id						= 0;
		mutable size_t m_attribIndex 	= 0;
	};
} // namespace Renderer

