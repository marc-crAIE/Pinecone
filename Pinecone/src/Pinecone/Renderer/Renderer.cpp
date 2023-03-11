#include "pcpch.h"
#include "Renderer.h"

#include "Pinecone/Renderer/RenderCommand.h"

namespace Pinecone
{
	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(int width, int height)
	{
	}
}