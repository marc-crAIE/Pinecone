#pragma once

#include "Pinecone/Asset/Asset.h"

#include <filesystem>

namespace Pinecone
{
	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::None; }
	};
}