#include "pcpch.h"
#include "Renderer.h"

#include "Pinecone/Renderer/RenderCommand.h"
#include "Pinecone/Renderer/Renderer2D.h"

namespace Pinecone
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetProjection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::OnWindowResize(int width, int height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}