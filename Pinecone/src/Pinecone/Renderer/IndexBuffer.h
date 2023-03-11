#pragma once
#include "Pinecone/Renderer/Buffer.h"

namespace Pinecone
{
	// Is currently only a 32-bit index buffer
	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}