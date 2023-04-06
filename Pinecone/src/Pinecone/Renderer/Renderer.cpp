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
		// Bind the shader and upload our view projection matrix that was setup inside the
		// BeginScene function from the specified camera
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

		// Draw our vertex array
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		// Set the view projection matrix that is passed to the shader
		s_SceneData->ViewProjectionMatrix = camera.GetProjection();
	}

	void Renderer::EndScene()
	{
		// End scene currently does nothing but this is for future proofing.
		// The use of it is recommended as it may cause problems if the 
		// engine is updated to give it use but the program does not currently 
		// use it
	}

	void Renderer::OnWindowResize(int width, int height)
	{
		// Set the OpenGL viewport to the specified width and height
		RenderCommand::SetViewport(0, 0, width, height);
	}
}