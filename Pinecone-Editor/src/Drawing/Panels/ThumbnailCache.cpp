#include "ThumbnailCache.h"

#include <Pinecone/Asset/TextureImporter.h>

namespace Pinecone
{
	ThumbnailCache::ThumbnailCache(Ref<Project> project)
		: m_Project(project)
	{
		// TODO: Maybe move this ti a cache dir?
		m_ThumbnailCachePath = m_Project->GetAssetDirectory() / "Thumbnail.cache";
	}

	Ref<Texture2D> ThumbnailCache::GetOrCreateThumbnail(const std::filesystem::path& path)
	{
		// The process of how this works
		// 1. Read file timestamp
		// 2. Compare hashed timestamp with existing cached image (in memory first, then from cache file)
		// 3. If equal, return associated thumbnail, otherwise load asset from disk and generate thumbnail
		// 4. If generated new thumbnail, store in cache obviously

		auto absolutePath = m_Project->GetAssetAbsolutePath(path);
		std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(absolutePath);
		uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();

		if (m_CachedImages.find(path) != m_CachedImages.end())
		{
			auto& cachedImage = m_CachedImages.at(path);
			if (cachedImage.Timestamp == timestamp)
				return cachedImage.Image;
		}

		// Just pngs for now
		if (path.extension() != ".png")
			return nullptr;

		Ref<Texture2D> texture = TextureImporter::LoadTexture2D(absolutePath);
		if (!texture)
			return nullptr;

		auto& cachedImage = m_CachedImages[path];
		cachedImage.Timestamp = timestamp;
		cachedImage.Image = texture;
		return cachedImage.Image;
	}
}