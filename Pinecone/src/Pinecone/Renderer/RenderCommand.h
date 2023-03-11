#pragma once

namespace Pinecone
{
	class RenderCommand
	{
	public:
		static void Init();

		static void SetViewport(int x, int y, int width, int height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
	};
}