#pragma once

#include "Pinecone/Core/Buffer.h"

namespace Pinecone
{
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}