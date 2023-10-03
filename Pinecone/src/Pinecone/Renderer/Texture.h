#pragma once

#include "Pinecone/Core/Buffer.h"
#include "Pinecone/Asset/Asset.h"

namespace Pinecone
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class TextureFilter
	{
		NEAREST,
		LINEAR
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
		TextureFilter Filter = TextureFilter::LINEAR;

		TextureSpecification() = default;
		TextureSpecification(TextureFilter filter) : Filter(filter) {}
	};

	class Texture : public Asset
	{
	public:
		/// <summary>
		/// The Texture deconstructor
		/// </summary>
		virtual ~Texture() = default;

		virtual void SetData(Buffer data) = 0;

		/// <summary>
		/// Bind the texture to a texture slot
		/// </summary>
		/// <param name="slot">The texture slot to bind to</param>
		virtual void Bind(uint32_t slot = 0) const = 0;

		/// <summary>
		/// Get the width of the texture
		/// </summary>
		/// <returns>The texture width</returns>
		virtual uint32_t GetWidth() const = 0;
		/// <summary>
		/// Get the height of the texture
		/// </summary>
		/// <returns>The texture height</returns>
		virtual uint32_t GetHeight() const = 0;
		/// <summary>
		/// The ID for the texture
		/// </summary>
		/// <returns>The texture ID</returns>
		virtual uint32_t GetRendererID() const = 0;

		/// <summary>
		/// Get the texture specification
		/// </summary>
		/// <returns>The texture specification</returns>
		virtual const TextureSpecification& GetSpecification() const = 0;

		/// <summary>
		/// Is the texture loaded?
		/// </summary>
		/// <returns>True if the texture is loaded</returns>
		virtual bool IsLoaded() const = 0;

		/// <summary>
		/// Equals comparison operator
		/// </summary>
		/// <param name="other">The other texture</param>
		/// <returns>True if both textures are the same</returns>
		virtual bool operator==(const Texture& other) const = 0;
	};
}