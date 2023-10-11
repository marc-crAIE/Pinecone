#pragma once

#include <Pinecone.h>

namespace Pinecone
{
	class EditorPanel
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& e) {}
		virtual void OnProjectChanged(const Ref<Project>& project) {}
		virtual void SetSceneContext(const Ref<Scene>& context) {}
	};
}