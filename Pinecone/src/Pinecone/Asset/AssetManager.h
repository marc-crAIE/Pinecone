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
	};
}