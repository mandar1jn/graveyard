#pragma once

#include "Archive.h"

namespace AWExtractor
{
	struct FolderEntry
	{
		u32 nameHash = 0;
		s64 nextNeighbourFolder = -1;
		s64 previousFolder = -1;
		s64 nameOffset = -1;
		s64 nextLowerFolder = -1;
		s64 nextFile = -1;

		string name = "";
	};

	struct FileEntry
	{
		u32 nameHash = 0;
		s64 nextFile = -1;
		s64 previousFolder = -1;
		s64 nameOffset = -1;
		s64 offset = -1;
		s64 size = -1;
		u32 fileDataHash = 0;
		u64 writeTime = 0;

		string name = "";
	};

	class RMDPArchiveIndex : ExtractorCore::ReadableFile
	{
	public:
		
		RMDPArchiveIndex(string path);

		bool bigEndian = false;
		u32 version = 0;
		u32 folderCount = 0;
		u32 fileCount = 0;

		u32 nameContainerSize = 0;

		string pathPrefix = "";

		// if remastered == true, assume version == 2
		bool remastered = false;

		std::vector<FolderEntry> folders = std::vector<FolderEntry>();
		std::vector<FileEntry> files = std::vector<FileEntry>();
	};

	class RMDPArchive : public ExtractorCore::Archive
	{
	public:
		RMDPArchive(string path);

		string fileName;

		void Draw() override;

		RMDPArchiveIndex index;

	private:
		void DrawFolder(s64 folderIndex);
	};
}