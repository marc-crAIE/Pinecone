#pragma once

#include "Pinecone/Core/UUID.h"
#include "Pinecone/Asset/AssetTypes.h"

namespace Pinecone
{
	using AssetHandle = UUID;

	class Asset
	{
	public:
		AssetHandle Handle; // Generate handle

		static AssetType GetStaticType() { return AssetType::None; }
		virtual AssetType GetType() const { return GetStaticType(); }

		virtual bool operator==(const Asset& other) const
		{
			return Handle == other.Handle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}
	};
}