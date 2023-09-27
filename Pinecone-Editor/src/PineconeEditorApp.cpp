#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pinecone
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

	Application* Pinecone::CreateApplication()
	{
		return new PineconeEditor();
	}
}