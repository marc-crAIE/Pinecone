#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pinecone
{
	class PineconeEditor : public Application
	{
	public:
		PineconeEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new RuntimeLayer());
		}

		~PineconeEditor()
		{}
	};

	Application* Pinecone::CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Pinecone Editor";
		spec.CommandLineArgs = args;

		return new PineconeEditor(spec);
	}
}