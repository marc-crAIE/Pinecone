#include "pcpch.h"
#include "UniformBuffer.h"

#include <glad/glad.h>

namespace Pinecone
{
	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
	{
		PC_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	UniformBuffer::~UniformBuffer()
	{
		PC_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		PC_PROFILE_FUNCTION();

		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		return CreateRef<UniformBuffer>(size, binding);
	}
}