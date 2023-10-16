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
			PushLayer(new EditorLayer());
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