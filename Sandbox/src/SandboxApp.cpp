#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "SandboxLayer.h"

using namespace Pinecone;

namespace Sandbox
{
	class SandboxApp : public Application
	{
	public:
		SandboxApp()
			: Application()
		{
			PushLayer(new SandboxLayer());
		}
	};
}

Application* Pinecone::CreateApplication()
{
	return new Sandbox::SandboxApp();
}