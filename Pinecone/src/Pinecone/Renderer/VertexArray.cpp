#include "pcpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Pinecone
{
	/// <summary>
	/// Converts our Pinecone shader datat type to a OpenGL one
	/// </summary>
	/// <param name="type">The shader data type</param>
	/// <returns>The OpenGL version of the shader data type</returns>
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		// Basically just return the OpenGL type of what our shader data type is
		// There are really only 3 that we care about currently. 
		// GL_FLOAT, GL_INT and GL_BOOL
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		// The shader data type is not supported, this shouldn't occur if used properly
		PC_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		PC_PROFILE_FUNCTION();

		// Create the vertex array and store its ID generated from OpenGL
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		PC_PROFILE_FUNCTION();

		// Delete the vertex array
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		PC_PROFILE_FUNCTION();

		// Bind the vertex array with our ID
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		PC_PROFILE_FUNCTION();

		// Unbind our vertex array
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		PC_PROFILE_FUNCTION();

		// Ensure that the vertex buffer has a buffer layout, the buffer layout is necessary to tell OpenGL
		// the location and size of the data in the array
		PC_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		// Bind the vertex array and the vertex buffer to setup the layout
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// Get the buffer layout and pass the layout data to OpenGL
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				PC_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		// Add the vertex buffer data
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		PC_PROFILE_FUNCTION();

		// Bind the vertex array and the index buffer
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		// Set the index buffer data
		m_IndexBuffer = indexBuffer;
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}
}