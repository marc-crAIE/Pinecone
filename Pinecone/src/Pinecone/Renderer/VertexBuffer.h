#pragma once

#include "Pinecone/Renderer/Buffer.h"

namespace Pinecone
{
	class VertexBuffer
	{
	public:
		/// <summary>
		/// The VertexBuffer constructor that takes in the total size of the vertex data.
		/// The size of the data must be that of the number of vertices multiplied by the
		/// total size of the data for each vertex.
		/// </summary>
		/// <param name="size">The size of the vertex data</param>
		VertexBuffer(uint32_t size);
		/// <summary>
		/// The VertexBuffer constructor that takes in vertex data (as a float array) and
		/// the total size of the data (not the number of vertices).
		/// The size of the data must be that of the number of vertices multiplied by the
		/// total size of the data for each vertex.
		/// </summary>
		/// <param name="vertices">The vertex data</param>
		/// <param name="size">The size of the vertex data</param>
		VertexBuffer(float* vertices, uint32_t size);
		/// <summary>
		/// The VertexBuffer deconstructor
		/// </summary>
		~VertexBuffer();

		/// <summary>
		/// Bind the vertex buffer
		/// </summary>
		void Bind() const;
		/// <summary>
		/// Unbind the vertex buffer
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// Set the vertex data with a given size.
		/// The size of the data must be that of the number of vertices multiplied by the
		/// total size of the data for each vertex.
		/// </summary>
		/// <param name="data">The vertex data</param>
		/// <param name="size">The size of the vertex data</param>
		void SetData(const void* data, uint32_t size) ;

		/// <summary>
		/// Returns the layout of the vertex data
		/// </summary>
		/// <returns>The vertex data layout</returns>
		const BufferLayout& GetLayout() const { return m_Layout; }
		/// <summary>
		/// Sets the laout of the vertex data. This must be done before adding the vertex 
		/// buffer to a vertex array.
		/// </summary>
		/// <param name="layout">The vertex buffer layout</param>
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		/// <summary>
		/// Creates a smart shared pointer to a new VertexBuffer of a given size
		/// </summary>
		/// <param name="size">The size of the vertex data</param>
		/// <returns>A shared pointer to a new VertexBuffer</returns>
		static Ref<VertexBuffer> Create(uint32_t size);
		/// <summary>
		/// Creates a smart shared pointer to a new VertexBuffer with some data of a given size
		/// </summary>
		/// <param name="vertices">The vertex data</param>
		/// <param name="size">The size of the vertex data</param>
		/// <returns>A shared pointer to a new VertexBuffer</returns>
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};
}