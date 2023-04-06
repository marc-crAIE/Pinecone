#pragma once

#include "Pinecone/Renderer/IndexBuffer.h"
#include "Pinecone/Renderer/VertexBuffer.h"

#include <memory>

namespace Pinecone
{
	class VertexArray
	{
	public:
		/// <summary>
		/// The VertexArray constructor
		/// </summary>
		VertexArray();
		/// <summary>
		/// The VertexArray destructor
		/// </summary>
		~VertexArray();

		/// <summary>
		/// Bind the vertex array
		/// </summary>
		void Bind() const;
		/// <summary>
		/// Unbind the vertex array
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// Add the vertex data to the vertex array. This can be called as long as
		/// the vertex data has a buffer layout.
		/// </summary>
		/// <param name="vertexBuffer">The vertex data to add</param>
		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		/// <summary>
		/// Set the index data for the vertex array
		/// </summary>
		/// <param name="indexBuffer">The index data</param>
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

		/// <summary>
		/// Get the vertex data stored in the vertex array
		/// </summary>
		/// <returns>The vertex data</returns>
		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		/// <summary>
		/// Get the index data stored in the vertex array
		/// </summary>
		/// <returns>The index data</returns>
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		/// <summary>
		/// Create a smart shared pointer to a new blank VertexArray
		/// </summary>
		/// <returns>A shared spointer to a new VertexArray</returns>
		static Ref<VertexArray> Create();
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}