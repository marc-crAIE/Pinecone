#include "pcpch.h"
#include "EditorAssetManager.h"

#include "Pinecone/Asset/AssetImporter.h"
#include "Pinecone/Asset/AssetExtensions.h"

#include "Pinecone/Project/Project.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Pinecone
{
	static AssetMetadata s_NullMetadata;

	YAML::Emitter& operator<<(YAML::Emitter& out, const std::string_view& v)
	{
		out << std::string(v.data(), v.size());
		return out;
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle) const
	{
		// Check if the asset handle is from memory
		if (IsMemoryAsset(handle))
			return m_MemoryAssets.at(handle);

		// Check if the asset handle is valid
		if (!IsAssetHandleValid(handle))
			return nullptr;

		// Check if the asset needs to be loaded (and if so, load it)
		Ref<Asset> asset;
		if (IsAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			// Load the asset
			const AssetMetadata& metadata = GetMetadata(handle);
			asset = AssetImporter::ImportAsset(handle, metadata);
			if (!asset)
			{
				// The import failed!
				PC_CORE_ERROR("EditorAssetManager::GetAsset - asset import failed!");
			}
		}

		// Return the asset
		return asset;
	}

	void EditorAssetManager::AddMemoryOnlyAsset(Ref<Asset> asset)
	{
		AssetMetadata metadata;
		metadata.Handle = asset->Handle;
		metadata.IsDataLoaded = true;
		metadata.Type = asset->GetType();
		metadata.IsMemoryAsset = true;
		m_AssetRegistry[metadata.Handle] = metadata;

		m_MemoryAssets[asset->Handle] = asset;
	}

	bool EditorAssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool EditorAssetManager::IsMemoryAsset(AssetHandle handle) const
	{
		return m_MemoryAssets.find(handle) != m_MemoryAssets.end();
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	AssetHandle EditorAssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		AssetHandle handle; // Generate new handle
		AssetMetadata metadata;
		metadata.FilePath = filepath;
		metadata.Type = GetAssetTypeFromExtension(filepath.extension().string());
		Ref<Asset> asset = AssetImporter::ImportAsset(handle, metadata);
		asset->Handle = handle;
		if (asset)
		{
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = metadata;
			SerializeAssetRegistry();
			return handle;
		}
		return 0;
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle) const
	{
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return s_NullMetadata;

		return it->second;
	}

	AssetType EditorAssetManager::GetAssetTypeFromExtension(const std::string& extension)
	{
		// TODO: Ensure that the extension is lower case
		if (s_AssetExtensionMap.find(extension) == s_AssetExtensionMap.end())
			return AssetType::None;

		return s_AssetExtensionMap.at(extension.c_str());
	}

	void EditorAssetManager::SerializeAssetRegistry()
	{
		auto path = Project::GetAssetRegistryPath();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;

			out << YAML::BeginSeq;
			for (const auto& [handle, metadata] : m_AssetRegistry)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				std::string filepathStr = metadata.FilePath.generic_string();
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(path);
		fout << out.c_str();
	}

	bool EditorAssetManager::DeserializeAssetRegistry()
	{
		auto path = Project::GetAssetRegistryPath();
		YAML::Node data;
		try
		{
			if (!std::filesystem::exists(path))
				return false;

			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			PC_CORE_ERROR("Failed to load project file '{0}'\n     {1}", path, e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;

		for (const auto& node : rootNode)
		{
			AssetHandle handle = node["Handle"].as<uint64_t>();
			auto& metadata = m_AssetRegistry[handle];
			metadata.FilePath = node["FilePath"].as<std::string>();
			metadata.Type = Utils::AssetTypeFromString(node["Type"].as<std::string>());
		}

		return true;
	}
}