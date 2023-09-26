#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

using namespace Pinecone;

#include "EditorLayer.h"

namespace Pinecone_Editor
{
	class PineconeEditor : public Application
	{
	public:
		PineconeEditor()
			: Application()
		{
			PushLayer(new EditorLayer());
		}

		~PineconeEditor()
		{}
	};
}

Application* Pinecone::CreateApplication()
{
	return new Pinecone_Editor::PineconeEditor();
}