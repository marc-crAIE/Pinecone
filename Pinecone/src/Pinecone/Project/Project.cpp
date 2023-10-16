#include "pcpch.h"
#include "Project.h"

#include "Pinecone/Project/ProjectSerializer.h"

namespace Pinecone
{
	std::filesystem::path Project::GetAssetAbsolutePath(const std::filesystem::path& path)
	{
		return GetAssetDirectory() / path;
	}

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = CreateRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			std::shared_ptr<EditorAssetManager> editorAssetManager = std::make_shared<EditorAssetManager>();
			s_ActiveProject->m_AssetManager = editorAssetManager;
			editorAssetManager->DeserializeAssetRegistry();
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path, bool overwitePath)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			if (overwitePath)
				s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}
}