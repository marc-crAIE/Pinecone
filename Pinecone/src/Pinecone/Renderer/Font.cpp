#include "pcpch.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

#include "Pinecone/Renderer/MSDFData.h"

namespace Pinecone {

	Ref<Font> Font::s_DefaultFont = nullptr;

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		PC_PROFILE_FUNCTION();

		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		{
			PC_PROFILE_SCOPE("GenerateFontAtlas");
			generator.setAttributes(attributes);
			generator.setThreadCount(8);
			generator.generate(glyphs.data(), (int)glyphs.size());
		}

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		{
			PC_PROFILE_SCOPE("CreateFontTexture");
			TextureSpecification spec;
			spec.Width = bitmap.width;
			spec.Height = bitmap.height;
			spec.Format = ImageFormat::RGB8;
			spec.GenerateMips = false;
			spec.Filter = TextureFilter::LINEAR;

			Ref<Texture2D> texture = Texture2D::Create(spec);
			texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
			return texture;
		}
	}

	Font::Font(const std::filesystem::path& filepath)
		: m_Data(new MSDFData())
	{
		PC_PROFILE_FUNCTION();

		// Initialize the FreeType font library
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		PC_CORE_ASSERT(ft);

		std::string fileString = filepath.string();

		// Load the font from the passed in file path
		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
		if (!font)
		{
			PC_CORE_ERROR("Failed to load font: {}", fileString);
			return;
		}

		// Stores the first and last character values
		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		// Use a character range between 32 (space) and 255 as characters before 
		// 32 are all special characters that cannot be seen.
		// Ref: https://www.rapidtables.com/code/text/ascii-table.html
		static const CharsetRange charsetRanges[] =
		{
			{ 0x0020, 0x00FF }
		};

		// Add the characters in our charset-range to the MSDF atlas charset
		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}

		// Initialize our font geometry
		double fontScale = 1.0;
		m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
		// Load the font glyphs from our charset into our font geometry
		int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
		PC_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
		PC_CORE_ASSERT(remaining == 0);

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
			}, m_Data->Glyphs.size()).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}

		m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("FontTest", (float)emSize, m_Data->Glyphs, m_Data->FontGeometry, width, height);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		PC_PROFILE_FUNCTION();

		delete m_Data;
	}

	Ref<Font> Font::GetDefault()
	{
		if (!s_DefaultFont)
			s_DefaultFont = Create("assets/fonts/opensans/OpenSans-Regular.ttf");

		return s_DefaultFont;
	}

	Ref<Font> Font::Create(const std::filesystem::path& font)
	{
		return CreateRef<Font>(font);
	}
}