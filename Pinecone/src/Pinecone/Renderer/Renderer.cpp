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

	void Renderer::OnWindowResize(int width, int height)
	{
	}
}