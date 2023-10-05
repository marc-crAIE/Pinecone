#pragma once

#include "Pinecone/Asset/AssetManagerBase.h"
#include "Pinecone/Asset/AssetMetadata.h"

#include <map>

namespace Pinecone
{
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) const override;

		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;

		AssetHandle ImportAsset(const std::filesystem::path& filepath) override;

		const AssetMetadata& GetMetadata(AssetHandle handle) const;

		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }

		void SerializeAssetRegistry();
		bool DeserializeAssetRegistry();
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
	};
}