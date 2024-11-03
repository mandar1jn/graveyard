#pragma once
#include <string>
#include <vector>
#include <files/RMDPArchive.h>

namespace AWExtractor
{

	class ResourceManager
	{
	public:
		ResourceManager();

		void LoadRMDPArchive(string path);

		RMDPArchive* GetArchiveForFile(BinIndexFile* file);
		RMDPArchive* GetArchiveForFolder(BinIndexFolder* folder);

		static ResourceManager* Instance;
		std::vector<RMDPArchive> rmdpArchives;

	};

}