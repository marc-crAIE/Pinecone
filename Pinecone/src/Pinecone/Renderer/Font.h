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
		/// <summary>
		/// The Font constructor that takes in a path to the font file
		/// </summary>
		/// <param name="font">The font file path</param>
		Font(const std::filesystem::path& font);
		/// <summary>
		/// The Font deconstructor
		/// </summary>
		~Font();

		/// <summary>
		/// Get the MSDF (multi-channel signed distance field) data created fron the font file.
		/// </summary>
		/// <remarks>
		/// Reference https://github.com/Chlumsky/msdfgen
		/// </remarks>
		/// <returns>The font MSDF data</returns>
		const MSDFData* GetMSDFData() const { return m_Data; }
		/// <summary>
		/// Get the font atlas texture generated from the font file
		/// </summary>
		/// <returns>The font atlas texture</returns>
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		/// <summary>
		/// Get the default font
		/// </summary>
		/// <returns>The default font</returns>
		static Ref<Font> GetDefault();

		/// <summary>
		/// Create a smart shared pointer to a new Font
		/// </summary>
		/// <param name="font">The font file path</param>
		/// <returns>A shared pointer to a new Font</returns>
		static Ref<Font> Create(const std::filesystem::path& font);
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	private:
		static Ref<Font> s_DefaultFont;
	};
}