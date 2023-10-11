#pragma once

#include "EditorPanel.h"
#include "ThumbnailCache.h"

#include <map>
#include <set>
#include <filesystem>

#include <Pinecone/Core/Base.h>
#include <Pinecone/Renderer/Texture2D.h>

namespace Pinecone
{
	class ContentBrowserPanel : public EditorPanel
	{
	public:
		ContentBrowserPanel(Ref<Project> project);

		void OnImGuiRender() override;

		void Open() { m_Open = true; }
	private:
		void RefreshAssetTree();
	private:
		Ref<Project> m_Project;
		Ref<ThumbnailCache> m_ThumbnailCache;

		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_SceneIcon;

		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FileEmptyIcon;
		Ref<Texture2D> m_FileGLSLIcon;

		struct TreeNode
		{
			std::filesystem::path Path;
			AssetHandle Handle = 0;

			uint32_t Parent = (uint32_t)-1;
			std::map<std::filesystem::path, uint32_t> Children;

			TreeNode(const std::filesystem::path& path, AssetHandle handle)
				: Path(path), Handle(handle) {}
		};

		std::vector<TreeNode> m_TreeNodes;

		std::map<std::filesystem::path, std::vector<std::filesystem::path>> m_AssetTree;

		enum class Mode
		{
			Asset = 0, FileSystem = 1
		};

		Mode m_Mode = Mode::Asset;

		bool m_Open = true;
	};
}