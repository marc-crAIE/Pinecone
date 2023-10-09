#pragma once
#include "Texture.h"

typedef unsigned int GLenum;

namespace Pinecone
{
	class Texture2D : public Texture
	{
	public:

		Texture2D(const TextureSpecification& specification, Buffer data = Buffer());
		/// <summary>
		/// The Texture deconstructor
		/// </summary>
		~Texture2D() override;

		virtual void SetData(Buffer data) override;
		/// <summary>
		/// Bind the texture to a texture slot
		/// </summary>
		/// <param name="slot">The texture slot to bind to</param>
		void Bind(uint32_t slot = 0) const override;

		/// <summary>
		/// Get the width of the texture
		/// </summary>
		/// <returns>The texture width</returns>
		uint32_t GetWidth() const override { return m_Width; }
		/// <summary>
		/// Get the height of the texture
		/// </summary>
		/// <returns>The texture height</returns>
		uint32_t GetHeight() const override { return m_Height; }
		/// <summary>
		/// The ID for the texture created by OpenGL
		/// </summary>
		/// <returns>The texture ID</returns>
		uint32_t GetRendererID() const override { return m_RendererID; }

		/// <summary>
		/// Get the texture specification
		/// </summary>
		/// <returns>The texture specification</returns>
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		/// <summary>
		/// Is the texture loaded?
		/// </summary>
		/// <returns>True if the texture is loaded</returns>
		bool IsLoaded() const override { return m_IsLoaded; }

		/// <summary>
		/// Equals comparison operator
		/// </summary>
		/// <param name="other">The other texture</param>
		/// <returns>True if both textures are the same</returns>
		bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

		static Ref<Texture2D> Create(const TextureSpecification& specification, Buffer data = Buffer());

		static AssetType GetStaticType() { return AssetType::Texture2D; }
		virtual AssetType GetType() const override { return GetStaticType(); }
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;

		TextureSpecification m_Specification;

		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
	};
}