#pragma once

#include "Pinecone/Asset/Asset.h"

#include <filesystem>

namespace Pinecone
{
	struct AssetMetadata
	{
		AssetHandle Handle = 0;
		AssetType Type = AssetType::None;

		std::filesystem::path FilePath;
		bool IsDataLoaded = false;
		bool IsMemoryAsset = false;

		bool IsValid() const { return Handle != 0 && !IsMemoryAsset; }

		operator bool() const { return Type != AssetType::None; }
	};
}