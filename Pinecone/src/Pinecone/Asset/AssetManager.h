#pragma once

#include "Pinecone/Asset/AssetManagerBase.h"

#include "Pinecone/Project/Project.h"

namespace Pinecone
{
	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			Ref<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		template<typename T>
		static AssetHandle AddMemoryOnlyAsset(Ref<T> asset)
		{
			static_assert(std::is_base_of<Asset, T>::value, "AddMemoryOnlyAsset only works for types derived from Asset");
			asset->Handle = AssetHandle();

			Project::GetActive()->GetAssetManager()->AddMemoryOnlyAsset(asset);
			return asset->Handle;
		}

		static bool IsAssetHandleValid(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
		}

		static bool IsAssetLoaded(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
		}

		static AssetType GetAssetType(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
		}
	};
}