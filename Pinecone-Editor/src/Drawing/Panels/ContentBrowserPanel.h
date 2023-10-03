#pragma once

#include <filesystem>

#include <Pinecone/Core/Base.h>
#include <Pinecone/Renderer/Texture2D.h>

namespace Pinecone
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

		void Open() { m_Open = true; }
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_SceneIcon;

		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FileEmptyIcon;
		Ref<Texture2D> m_FileGLSLIcon;

		bool m_Open = true;
	};
}