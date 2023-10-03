#pragma once

#include "Pinecone/Asset/AssetMetadata.h"

namespace Pinecone
{
	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};
}