#include "ResourceManager.h"

namespace AWExtractor
{
	ResourceManager* ResourceManager::Instance;

	ResourceManager::ResourceManager()
	{
		Instance = this;
		rmdpArchives = std::vector<RMDPArchive>();
	}

	void ResourceManager::LoadRMDPArchive(string path)
	{
		for (const auto& archive : rmdpArchives)
		{
			if (archive.path == path)
				return;
		}

		rmdpArchives.push_back(RMDPArchive(path));
	}
	RMDPArchive* ResourceManager::GetArchiveForFile(BinIndexFile* file)
	{
		for (auto& archive : rmdpArchives)
		{
			auto result = std::find(archive.binData.files.begin(), archive.binData.files.end(), file);

			if (result != archive.binData.files.end())
			{
				return &archive;
			}
		}

		return nullptr;
	}
	RMDPArchive* ResourceManager::GetArchiveForFolder(BinIndexFolder* folder)
	{
		for (auto& archive : rmdpArchives)
		{
			auto result = std::find(archive.binData.folders.begin(), archive.binData.folders.end(), folder);

			if (result != archive.binData.folders.end())
			{
				return &archive;
			}
		}

		return nullptr;
	}
}