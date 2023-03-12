#include "SandboxLayer.h"

namespace Sandbox
{
	SandboxLayer::SandboxLayer()
		: Layer("SandboxLayer"), m_Camera(glm::ortho(- 1.6f, 1.6f, -0.9f, 0.9f))
	{ }

	void SandboxLayer::OnAttach()
	{
		m_VertexArray = VertexArray::Create();

		// Our vertex data (Vec3 (position), Vec4 (Colour))
		// Note: Nothing happens with colour at the moment as we don't yet have shaders!
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		// Create the vertex buffer
		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Create the index buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Shader::Create(vertexSrc, fragmentSrc);

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	}

	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate()
	{
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_Shader, m_VertexArray);

		Renderer::EndScene();
	}

	void SandboxLayer::OnEvent(Event& e)
	{
	}
}