#pragma once

#include <Pinecone/Project/Project.h>
#include <Pinecone/Renderer/Texture2D.h>

namespace Pinecone
{
	struct ThumbnailImage
	{
		uint64_t Timestamp;
		Ref<Texture2D> Image;
	};

	class ThumbnailCache
	{
	public:
		ThumbnailCache(Ref<Project> project);

		Ref<Texture2D> GetOrCreateThumbnail(const std::filesystem::path& path);
	private:
		Ref<Project> m_Project;

		std::map<std::filesystem::path, ThumbnailImage> m_CachedImages;
		std::filesystem::path m_ThumbnailCachePath;
	};
}