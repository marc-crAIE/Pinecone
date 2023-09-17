#include "pcpch.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Pinecone
{
	VertexBuffer::VertexBuffer(uint32_t size)
	{
		PC_PROFILE_FUNCTION();

		// Create a new array buffer and store the ID
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		// Set the size of the array buffer with no data, tell OpenGL we want to dynamically allocate it
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		PC_PROFILE_FUNCTION();

		// Create a new array buffer and store the ID
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		// Set the size of the array with our vertex data and tell OpenGL to statically allocate it
		// TODO: Maybe tell OpenGL to dynamically allocate as we can set the data after creating to a
		//		 different size with the VertexBuffer::SetData function
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		PC_PROFILE_FUNCTION();

		// Delete the vertex buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		PC_PROFILE_FUNCTION();

		// Bind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		PC_PROFILE_FUNCTION();

		// Unbind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		PC_PROFILE_FUNCTION();

		// Bind the vertex buffer then set its data 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<VertexBuffer>(size);
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<VertexBuffer>(vertices, size);
	}
}