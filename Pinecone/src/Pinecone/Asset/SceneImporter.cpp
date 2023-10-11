#include "pcpch.h"
#include "SceneImporter.h"

#include "Pinecone/Project/Project.h"
#include "Pinecone/Scene/SceneSerializer.h"

#include <stb_image.h>

namespace Pinecone
{
	Ref<Scene> SceneImporter::ImportScene(AssetHandle handle, const AssetMetadata& metadata)
	{
		PC_PROFILE_FUNCTION();

		return LoadScene(Project::GetAssetDirectory() / metadata.FilePath);
	}

	Ref<Scene> SceneImporter::LoadScene(const std::filesystem::path& path)
	{
		PC_PROFILE_FUNCTION();

		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.Deserialize(path);
		return scene;
	}

	void SceneImporter::SaveScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(Project::GetAssetDirectory() / path);
	}
}