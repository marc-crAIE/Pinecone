#include "ContentBrowserPanel.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

namespace Pinecone
{
	// Once we have projects, change this
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/Panels/ContentBrowser/DirectoryIcon.png");
		m_SceneIcon = Texture2D::Create("Resources/Icons/Panels/ContentBrowser/SceneIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/Panels/ContentBrowser/FileIcon.png");
		m_FileEmptyIcon = Texture2D::Create("Resources/Icons/Panels/ContentBrowser/FileEmptyIcon.png");
		m_FileGLSLIcon = Texture2D::Create("Resources/Icons/Panels/ContentBrowser/FileGLSLIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (!m_Open)
			return;

		ImGui::Begin("Content Browser", &m_Open);

		if (ImGui::Button("<-"))
		{
			if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		ImGui::Separator();

		static float padding = 4.0f;
		static float thumbnailSize = 72.0f;
		float cellSize = thumbnailSize + padding * 2;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.file_size() > 0 ? m_FileIcon : m_FileEmptyIcon;
			if (!directoryEntry.is_directory() && relativePath.has_extension())
			{
				std::string extension = relativePath.extension().string();
				if (extension == ".ascene")
					icon = m_SceneIcon;
				else if (extension == ".glsl")
					icon = m_FileGLSLIcon;
			}
			else
				icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_AssetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Image((ImTextureID)icon->GetRendererID(), ImVec2{ 16.0f, 16.0f }, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Text(path.string().c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		// TODO: status bar
		ImGui::End();
	}
}