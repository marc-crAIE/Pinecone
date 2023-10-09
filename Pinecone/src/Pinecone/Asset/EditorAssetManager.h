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

		virtual void AddMemoryOnlyAsset(Ref<Asset> asset) override;

		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsMemoryAsset(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;

		AssetHandle ImportAsset(const std::filesystem::path& filepath);

		const AssetMetadata& GetMetadata(AssetHandle handle) const;
		AssetType GetAssetTypeFromExtension(const std::string& extension);

		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }

		void SerializeAssetRegistry();
		bool DeserializeAssetRegistry();
	private:
		AssetMap m_LoadedAssets;
		AssetMap m_MemoryAssets;
		AssetRegistry m_AssetRegistry;
	};
}