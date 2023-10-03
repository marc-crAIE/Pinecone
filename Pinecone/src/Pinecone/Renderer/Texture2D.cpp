#include "pcpch.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Pinecone
{
	namespace Utils {

		static GLenum PineconeImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:		return GL_RGB;
			case ImageFormat::RGBA8:	return GL_RGBA;
			}

			PC_CORE_ASSERT(false);
			return 0;
		}

		static GLenum PineconeImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:		return GL_RGB8;
			case ImageFormat::RGBA8:	return GL_RGBA8;
			}

			PC_CORE_ASSERT(false);
			return 0;
		}

	}

	Texture2D::Texture2D(const TextureSpecification& specification, Buffer data)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		PC_PROFILE_FUNCTION();

		m_InternalFormat = Utils::PineconeImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::PineconeImageFormatToGLDataFormat(m_Specification.Format);

		// Create a new 2D texture based on data from our specification
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		GLenum filter = m_Specification.Filter == TextureFilter::NEAREST ? GL_NEAREST : GL_LINEAR;

		// Configure the parameters for the texture
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data)
			SetData(data);
	}

	Texture2D::~Texture2D()
	{
		PC_PROFILE_FUNCTION();

		// Delete the texture
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::SetData(Buffer data)
	{
		PC_PROFILE_FUNCTION();

		// Get the bits per pixel
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		// Make sure the size of the data will fill the data of the entire texture
		PC_CORE_ASSERT(data.Size == m_Width * m_Height * bpp, "Data must be entire texture!");
		// Set the data of the texture
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		PC_PROFILE_FUNCTION();

		// Bind the texture to the specified slot
		glBindTextureUnit(slot, m_RendererID);
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, Buffer data)
	{
		return CreateRef<Texture2D>(specification, data);
	}
}