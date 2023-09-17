#include "pcpch.h"
#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Pinecone
{
	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		PC_PROFILE_FUNCTION();

		// Create a new array buffer and store the ID
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		// Set the data in the array buffer to the given index data, tell OpenGL to dynamically allocate this
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		PC_PROFILE_FUNCTION();

		// Delete the index buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::Bind() const
	{
		PC_PROFILE_FUNCTION();

		// Bind the index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		PC_PROFILE_FUNCTION();

		// Unbind the index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return CreateRef<IndexBuffer>(indices, count);
	}
}