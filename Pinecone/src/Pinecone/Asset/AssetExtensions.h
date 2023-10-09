#pragma once

#include <unordered_map>

#include "Pinecone/Asset/AssetTypes.h"

namespace Pinecone
{
	inline static std::unordered_map<std::string, AssetType> s_AssetExtensionMap =
	{
		// Pinecone Types
		{ ".pscene", AssetType::Scene },

		// Textures
		{ ".png", AssetType::Texture2D },
		{ ".jpg", AssetType::Texture2D },
		{ ".jpeg", AssetType::Texture2D },
	};
}