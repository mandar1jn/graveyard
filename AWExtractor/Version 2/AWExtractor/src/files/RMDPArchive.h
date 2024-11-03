#pragma once
#include "common.h"
#include <utils/EndianReader.h>
#include <vector>
#include <unordered_map>

namespace AWExtractor
{
	struct BinIndexFolder
	{
		u32 nameHash;
		s64 nextNeighbourFolder;
		s64 previousFolder;
		u32 unknown;
		s64 nameOffset;
		string name = "";
		s64 nextLowerFolder;
		s64 nextFile;
	};

	inline bool operator==(const BinIndexFolder& lhs, const BinIndexFolder* rhs)
	{
		return &lhs == rhs;
	}

	struct BinIndexFile
	{
		u32 nameHash;
		s64 nextFile;
		s64 previousFolder;
		u32 unknown;
		s64 nameOffset;
		string name = "";
		s64 offset;
		s64 size;
		u32 fileDataHash;
		u64 writeTime = 0;
	};

	inline bool operator==(const BinIndexFile& lhs, const BinIndexFile* rhs)
	{
		return (lhs.name == rhs->name) && (lhs.fileDataHash == rhs->fileDataHash);
	}

	struct BinIndexData
	{
		bool bigEndian;
		u32 version;
		u32 folderCount;
		u32 fileCount;
		u32 nameContainerSize;
		string pathPrefix;
		std::vector<BinIndexFolder> folders;
		std::vector<BinIndexFile> files;
	};

	struct PackmetaFileEntry
	{
		string path;
		u32 offset;
	};

	struct PackmetaRIDEntry
	{
		rid rid;
		u32 offset;
	};

	struct PackmetaData
	{
		u32 pairCount;
		u32 version;
		u32 fileCount;
		u32 pathContainerSize;
		std::vector<PackmetaFileEntry> fileEntries;
		u32 ridCount;
		std::vector<PackmetaRIDEntry> ridEntries;
	};

	class RMDPArchive
	{
	public:
		RMDPArchive(string path);

		void ReadBinFile();
		void ReadBinHeaderV2(EndianReader* er);
		void ReadBinHeaderV7(EndianReader* er);
		void ReadBinHeaderV8(EndianReader* er);
		void ReadBinHeaderV9(EndianReader* er);

		void ReadPackmetaFile();

		string GetFilePath(BinIndexFile* file);

		void ExportFile(string path, BinIndexFile* file);

		void ExportFolder(string path, BinIndexFolder* folder);

		BinIndexData binData;
		PackmetaData packmetaData;

		std::unordered_map<string, rid> pathMap;

		std::unordered_map<rid, BinIndexFile*> assetFiles;

		string path;

	};

}