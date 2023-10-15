#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "RuntimeLayer.h"

class RuntimeApp : public Pinecone::Application
{
public:
	RuntimeApp(const Pinecone::ApplicationSpecification& specification, std::string_view projectPath)
		: Application(specification), m_ProjectPath(projectPath)
	{
		s_IsRuntime = true;
	}

	virtual void OnInit() override
	{
		PushLayer(new Pinecone::RuntimeLayer(m_ProjectPath));
	}
private:
	std::string m_ProjectPath;
};

Pinecone::Application* Pinecone::CreateApplication(ApplicationCommandLineArgs args)
{
	std::string_view projectPath = "App.pcproj";

#ifdef PC_DEBUG
	projectPath = "";
#endif

	Pinecone::ApplicationSpecification spec;
	spec.Name = "Pinecone Runtime";
	spec.CommandLineArgs = args;

	if (args.Count > 1)
		projectPath = args[1];

	return new RuntimeApp(spec, projectPath);
}