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

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();

		static Ref<Font> Create(const std::filesystem::path& font);
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	private:
		static Ref<Font> s_DefaultFont;
	};
}