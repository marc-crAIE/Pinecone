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

	Texture2D::Texture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		m_InternalFormat = Utils::PineconeImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::PineconeImageFormatToGLDataFormat(m_Specification.Format);

		// Create a new 2D texture based on data from our specification
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// Configure the parameters for the texture
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::string& filepath)
		: m_FilePath(filepath)
	{
		int width, height, channels;
		// Make sure the first pixel read from the texture is on the bottom left
		// Not doing this will make our textures appear to be upside down
		stbi_set_flip_vertically_on_load(1);
		// Load the texture data
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			// Figure out what the format of the texture is based on the amount of color channels
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			// Make sure the format is supported
			PC_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			// Create a 2D texture with the width, height, and the format from the loaded texture
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			// Configure the parameters for the texture
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Set the data of the texture
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			// Free the stb image data
			stbi_image_free(data);
		}
	}

	Texture2D::~Texture2D()
	{
		// Delete the texture
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		// Get the bits per pixel
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		// Make sure the size of the data will fill the data of the entire texture
		PC_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		// Set the data of the texture
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		// Bind the texture to the specified slot
		glBindTextureUnit(slot, m_RendererID);
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		return CreateRef<Texture2D>(specification);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		return CreateRef<Texture2D>(filepath);
	}
}