#include "ContentBrowserPanel.h"

#include "../UIWidgets.h"

#include <Pinecone/Project/Project.h>
#include <Pinecone/Asset/AssetManager.h>
#include <Pinecone/Asset/TextureImporter.h>
#include <Pinecone/ImGui/UICore.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Pinecone
{
	ContentBrowserPanel::ContentBrowserPanel(Ref<Project> project)
		: m_Project(project), m_ThumbnailCache(CreateRef<ThumbnailCache>(project)), m_BaseDirectory(m_Project->GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_TreeNodes.push_back(TreeNode(".", 0));

		m_DirectoryIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/ContentBrowser/DirectoryIcon.png");
		m_SceneIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/ContentBrowser/SceneIcon.png");
		m_FileIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/ContentBrowser/FileIcon.png");
		m_FileEmptyIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/ContentBrowser/FileEmptyIcon.png");
		m_FileGLSLIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/ContentBrowser/FileGLSLIcon.png");

		RefreshAssetTree();

		m_Mode = Mode::FileSystem;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_SizingFixedFit
			| ImGuiTableFlags_BordersInnerV;
		UI::PushID();
		if (ImGui::BeginTable(UI::GenerateID(), 2, tableFlags, ImVec2(0.0f, 0.0f)))
		{
			ImGui::TableSetupColumn("Outliner", 0, 200.0f);
			ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// Content Outliner

			ImGui::BeginChild("##folders_common");
			{
				if (!m_BaseDirectory.empty())
					RenderDirectoryHierarchy(m_BaseDirectory);
			}
			ImGui::EndChild();

			ImGui::TableSetColumnIndex(1);

			// Directory Content

			const float topBarHeight = 26.0f;
			const float bottomBarHeight = 32.0f;
			ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
			{

				const char* label = m_Mode == Mode::Asset ? "Asset" : "File";
				if (ImGui::Button(label))
				{
					m_Mode = m_Mode == Mode::Asset ? Mode::FileSystem : Mode::Asset;
				}

				if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
				{
					ImGui::SameLine();
					if (ImGui::Button("<-"))
					{
						m_CurrentDirectory = m_CurrentDirectory.parent_path();
					}
				}

				ImGui::Separator();

				static float padding = 16.0f;
				static float thumbnailSize = 64.0f;
				float cellSize = thumbnailSize + padding;

				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;

				ImGui::Columns(columnCount, 0, false);

				if (m_Mode == Mode::Asset)
				{
					bool assetDirectoryExists = true;
					TreeNode* node = &m_TreeNodes[0];

					auto currentDir = std::filesystem::relative(m_CurrentDirectory, Project::GetActiveAssetDirectory());
					for (const auto& p : currentDir)
					{
						// if only one level
						if (node->Path == currentDir)
							break;

						if (node->Children.find(p) != node->Children.end())
						{
							node = &m_TreeNodes[node->Children[p]];
							continue;
						}
						else
						{
							// can't find path
							assetDirectoryExists = false;
						}
					}

					if (assetDirectoryExists)
					{
						for (const auto& [item, treeNodeIndex] : node->Children)
						{
							bool isDirectory = std::filesystem::is_directory(Project::GetActiveAssetDirectory() / item);

							std::string itemStr = item.generic_string();

							ImGui::PushID(itemStr.c_str());

							// THUMBNAIL
							AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
							auto relativePath = m_Project->GetEditorAssetManager()->GetFilePath(handle);
							Ref<Texture2D> thumbnail = isDirectory ? m_DirectoryIcon : m_FileIcon;
							if (!isDirectory)
							{
								thumbnail = m_ThumbnailCache->GetOrCreateThumbnail(relativePath);
								if (!thumbnail)
									thumbnail = m_FileIcon;
							}

							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
							ImGui::ImageButton((ImTextureID)thumbnail->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

							if (ImGui::BeginPopupContextItem())
							{
								if (ImGui::MenuItem("Delete"))
								{
									PC_CORE_ASSERT(false, "Not implemented");
								}
								ImGui::EndPopup();
							}

							if (ImGui::BeginDragDropSource())
							{
								AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
								ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", &handle, sizeof(AssetHandle));
								ImGui::Image((ImTextureID)thumbnail->GetRendererID(), ImVec2{ 16.0f, 16.0f }, ImVec2(0, 1), ImVec2(1, 0));
								ImGui::SameLine();
								ImGui::Text(relativePath.string().c_str());
								ImGui::EndDragDropSource();
							}


							ImGui::PopStyleColor();
							if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
							{
								if (isDirectory)
									m_CurrentDirectory /= item.filename();
							}

							ImGui::TextWrapped(itemStr.c_str());

							ImGui::NextColumn();

							ImGui::PopID();
						}
					}
				}
				else
				{
					for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
					{
						const auto& path = directoryEntry.path();
						std::string filenameString = path.filename().string();

						ImGui::PushID(filenameString.c_str());

						// THUMBNAIL
						auto relativePath = std::filesystem::relative(path, Project::GetActiveAssetDirectory());
						Ref<Texture2D> thumbnail = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
						if (!directoryEntry.is_directory())
						{
							thumbnail = m_ThumbnailCache->GetOrCreateThumbnail(relativePath);
							if (!thumbnail)
								thumbnail = m_FileIcon;
						}

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
						ImGui::ImageButton((ImTextureID)thumbnail->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Import"))
							{
								Project::GetActive()->GetEditorAssetManager()->ImportAsset(relativePath);
								RefreshAssetTree();
							}
							ImGui::EndPopup();
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
				}
				ImGui::EndChild();
			}

			ImGui::EndTable();
		}
		UI::PopID();
		ImGui::End();
	}

	void ContentBrowserPanel::RefreshAssetTree()
	{
		const auto& assetRegistry = Project::GetActive()->GetEditorAssetManager()->GetAssetRegistry();
		for (const auto& [handle, metadata] : assetRegistry)
		{
			uint32_t currentNodeIndex = 0;

			for (const auto& p : metadata.FilePath)
			{
				auto it = m_TreeNodes[currentNodeIndex].Children.find(p.generic_string());
				if (it != m_TreeNodes[currentNodeIndex].Children.end())
				{
					currentNodeIndex = it->second;
				}
				else
				{
					// Add node
					TreeNode newNode(p, handle);
					newNode.Parent = currentNodeIndex;
					m_TreeNodes.push_back(newNode);

					m_TreeNodes[currentNodeIndex].Children[p] = m_TreeNodes.size() - 1;
					currentNodeIndex = m_TreeNodes.size() - 1;
				}
			}
		}
	}

	void ContentBrowserPanel::RenderDirectoryHierarchy(std::filesystem::path& path)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		for (const auto& dirEntry : std::filesystem::directory_iterator(path))
		{
			if (dirEntry.is_directory())
			{
				auto path = dirEntry.path();
				std::string name = path.filename().string();
				std::string id = name + "_TreeNode";
				bool opened = UI::DrawTreeNodeWithImage((ImTextureID)m_DirectoryIcon->GetRendererID(), (void*)ImGui::GetID(id.c_str()), flags, name.c_str());

				if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
				{
					m_CurrentDirectory = path;
				}

				if (opened)
				{
					RenderDirectoryHierarchy(path);
					ImGui::TreePop();
				}
			}
		}
	}
}