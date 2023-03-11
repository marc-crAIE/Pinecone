#pragma once

namespace Pinecone
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(int width, int height);
	};
}