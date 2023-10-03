#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "SandboxLayer.h"

using namespace Pinecone;

namespace Sandbox
{
	class SandboxApp : public Application
	{
	public:
		SandboxApp(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new SandboxLayer());
		}
	};
}

Application* Pinecone::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Pinecone-Editor";
	spec.CommandLineArgs = args;

	return new Sandbox::SandboxApp(spec);
}