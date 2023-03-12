#pragma once

#include "Pinecone/Renderer/VertexArray.h"
#include "Pinecone/Renderer/Shader.h"
#include "Pinecone/Renderer/Camera.h"

namespace Pinecone
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void OnWindowResize(int width, int height);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}