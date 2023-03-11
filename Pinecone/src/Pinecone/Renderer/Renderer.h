#pragma once

#include "Pinecone/Renderer/VertexArray.h"

namespace Pinecone
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void Submit(const Ref<VertexArray>& vertexArray);

		static void OnWindowResize(int width, int height);
	};
}