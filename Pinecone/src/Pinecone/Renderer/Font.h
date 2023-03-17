#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Renderer/Texture2D.h"

#include <filesystem>

namespace Pinecone
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}