#pragma once

#include "Pinecone/Asset/Asset.h"
#include "Pinecone/Asset/AssetMetadata.h"

#include "Pinecone/Scene/Scene.h"

namespace Pinecone
{
	class SceneImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);

		// Load from filepath
		static Ref<Scene> LoadScene(const std::filesystem::path& path);

		static void SaveScene(Ref<Scene> scene, const std::filesystem::path& path);
	};
}