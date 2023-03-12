#pragma once

#include "Pinecone.h"

using namespace Pinecone;

namespace Sandbox
{
	class SandboxLayer : public Layer
	{
	public:
		SandboxLayer();
		~SandboxLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate() override;
		void OnEvent(Event& e) override;
	private:
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Camera m_Camera;
	};
}