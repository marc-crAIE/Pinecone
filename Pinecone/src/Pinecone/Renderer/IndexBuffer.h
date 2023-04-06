#pragma once
#include "Pinecone/Renderer/Buffer.h"

namespace Pinecone
{
	// Is currently only a 32-bit index buffer
	class IndexBuffer
	{
	public:
		/// <summary>
		/// The IndexBuffer constructor that takes in the index data and the amount of
		/// indices in the index data given (not the total size of the index data).
		/// </summary>
		/// <param name="indices">The index data</param>
		/// <param name="count">The amount of indices</param>
		IndexBuffer(uint32_t* indices, uint32_t count);
		/// <summary>
		/// The IndexBuffer deconstructor
		/// </summary>
		~IndexBuffer();

		/// <summary>
		/// Bind the index buffer
		/// </summary>
		void Bind() const;
		/// <summary>
		/// Unbind the index buffer
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// Get the amount of indices in the index buffer
		/// </summary>
		/// <returns>The amount of indices</returns>
		uint32_t GetCount() const { return m_Count; }

		/// <summary>
		/// Create a smart shared pointer to a new IndexBuffer with index data and the amount of indices
		/// in the index data given (not the total size of the index data).
		/// </summary>
		/// <param name="indices">The index data</param>
		/// <param name="count">The amount of indices</param>
		/// <returns>A shared pointer to a new IndexBuffer</returns>
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}