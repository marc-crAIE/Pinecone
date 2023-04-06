#pragma once
#include "Texture.h"

typedef unsigned int GLenum;

namespace Pinecone
{
	class Texture2D : public Texture
	{
	public:
		/// <summary>
		/// The Texture2D constructor for creating a texture based on a texture specification
		/// </summary>
		/// <param name="specification">The texture specification</param>
		Texture2D(const TextureSpecification& specification);
		/// <summary>
		/// The Texture2D constructor for creating a texture from a file
		/// </summary>
		/// <param name="filepath">The file path to the texture file</param>
		Texture2D(const std::string& filepath);
		/// <summary>
		/// The Texture deconstructor
		/// </summary>
		~Texture2D() override;

		/// <summary>
		/// Set the texture data (pixels)
		/// </summary>
		/// <param name="data">The pixel data</param>
		/// <param name="size">The total size of the pixel data</param>
		void SetData(void* data, uint32_t size) override;
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
		/// Get the file path which the texture was created from
		/// </summary>
		/// <returns></returns>
		const std::string& GetFilePath() const override { return m_FilePath; }

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

		/// <summary>
		/// Create a smart shared pointer to a new Texture2D
		/// </summary>
		/// <param name="specification">The texture specification</param>
		/// <returns>A shared pointer to a new Texture2D</returns>
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		/// <summary>
		/// Create a smart shared pointer to a new Texture2D
		/// </summary>
		/// <param name="filepath">The file path to the texture file</param>
		/// <returns>A shared pointer to a new Texture2D</returns>
		static Ref<Texture2D> Create(const std::string& filepath);
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_FilePath;

		TextureSpecification m_Specification;

		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
	};
}