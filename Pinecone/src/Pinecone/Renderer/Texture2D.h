#pragma once
#include "Texture.h"

typedef unsigned int GLenum;

namespace Pinecone
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& filepath);
		~Texture2D() override;

		void SetData(void* data, uint32_t size) override;
		void Bind(uint32_t slot = 0) const override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_RendererID; }

		const std::string& GetFilePath() const override { return m_FilePath; }

		bool IsLoaded() const override { return m_IsLoaded; }

		bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& filepath);
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_FilePath;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
	};
}