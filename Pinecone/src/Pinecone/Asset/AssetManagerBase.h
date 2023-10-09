#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Asset/Asset.h"

#include <map>

namespace Pinecone
{
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;

	class AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) const = 0;

		virtual void AddMemoryOnlyAsset(Ref<Asset> asset) = 0;

		virtual bool IsAssetHandleValid(AssetHandle handle) const = 0;
		virtual bool IsMemoryAsset(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;
	};
}