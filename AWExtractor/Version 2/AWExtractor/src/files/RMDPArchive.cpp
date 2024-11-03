#include "RMDPArchive.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace AWExtractor
{
	RMDPArchive::RMDPArchive(string path)
	{
		this->path = path;

		ReadBinFile();

		if (binData.fileCount > 0 && binData.version >= 8)
		{

			ReadPackmetaFile();

			assetFiles = {};


			for (auto& file : binData.files)
			{
				string filePath = GetFilePath(&file);

				auto found = pathMap.find(filePath);
				if (found != pathMap.end())
				{
					assetFiles.insert(std::make_pair(found->second, &file));
				}
			}
		}
	}

	void RMDPArchive::ReadBinFile()
	{
		binData = {};

		EndianReader er(path.substr(0, path.find_last_of('.')).append(".bin"));

		binData.bigEndian = er.ReadBool();

		if (binData.bigEndian)
		{
			er.SetEndianness(Endianness::Big);
		}
		else
		{
			er.SetEndianness(Endianness::Little);
		}

		binData.version = er.ReadU32();

		if (binData.version == 2)
		{
			ReadBinHeaderV2(&er);
		}
		else if (binData.version == 7)
		{
			ReadBinHeaderV7(&er);
		}
		else if (binData.version == 8)
		{
			ReadBinHeaderV8(&er);
		}
		else if (binData.version == 9)
		{
			ReadBinHeaderV9(&er);
		}
		else
		{
			throw "Unsupported bin file header version";
		}
	}

	void RMDPArchive::ReadBinHeaderV2(EndianReader* er)
	{
		binData.folderCount = er->ReadU32();
		binData.fileCount = er->ReadU32();

		binData.folders.reserve(binData.folderCount);
		binData.files.reserve(binData.fileCount);

		binData.nameContainerSize = er->ReadU32();
		binData.pathPrefix = er->ReadNullTerminatedString();

		er->Skip(120); //unknown

		for (u32 i = 0; i < binData.folderCount; i++)
		{
			BinIndexFolder folder = {};

			folder.nameHash = er->ReadU32();

			folder.nextNeighbourFolder = er->ReadS32();
			folder.previousFolder = er->ReadS32();

			folder.unknown = er->ReadU32();

			folder.nameOffset = er->ReadS32();

			if (folder.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - folder.nameOffset), SeekDirection::End);
				folder.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			folder.nextLowerFolder = er->ReadS32();
			folder.nextFile = er->ReadS32();

			binData.folders.push_back(folder);
		}

		for (u32 i = 0; i < binData.fileCount; i++)
		{
			BinIndexFile file = {};

			file.nameHash = er->ReadU32();
			file.nextFile = er->ReadS32();
			file.previousFolder = er->ReadS32();
			file.unknown = er->ReadU32();
			file.nameOffset = er->ReadS32();

			if (file.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - file.nameOffset), SeekDirection::End);
				file.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			file.offset = er->ReadS64();
			file.size = er->ReadS64();
			file.fileDataHash = er->ReadU32();

			binData.files.push_back(file);
		}
	}

	void RMDPArchive::ReadBinHeaderV7(EndianReader* er)
	{
		binData.folderCount = er->ReadU32();
		binData.fileCount = er->ReadU32();

		binData.folders.reserve(binData.folderCount);
		binData.files.reserve(binData.fileCount);

		er->Skip(8); //unknown

		binData.nameContainerSize = er->ReadU32();
		binData.pathPrefix = er->ReadNullTerminatedString();

		er->Skip(120); //unknown

		for (u32 i = 0; i < binData.folderCount; i++)
		{
			BinIndexFolder folder = {};

			folder.nameHash = er->ReadU32();

			folder.nextNeighbourFolder = er->ReadS32();
			folder.previousFolder = er->ReadS32();

			folder.unknown = er->ReadU32();

			folder.nameOffset = er->ReadS32();

			if (folder.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - folder.nameOffset), SeekDirection::End);
				folder.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			folder.nextLowerFolder = er->ReadS32();
			folder.nextFile = er->ReadS32();

			binData.folders.push_back(folder);
		}

		for (u32 i = 0; i < binData.fileCount; i++)
		{
			BinIndexFile file = {};

			file.nameHash = er->ReadU32();
			file.nextFile = er->ReadS32();
			file.previousFolder = er->ReadS32();
			file.unknown = er->ReadU32();
			file.nameOffset = er->ReadS32();

			if (file.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - file.nameOffset), SeekDirection::End);
				file.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			file.offset = er->ReadS64();
			file.size = er->ReadS64();
			file.fileDataHash = er->ReadU32();
			file.writeTime = er->ReadU64();

			binData.files.push_back(file);
		}
	}

	void RMDPArchive::ReadBinHeaderV8(EndianReader* er)
	{
		binData.folderCount = er->ReadU32();
		binData.fileCount = er->ReadU32();

		binData.folders.reserve(binData.folderCount);
		binData.files.reserve(binData.fileCount);

		er->Skip(8); // unknown

		binData.nameContainerSize = er->ReadU32();

		binData.pathPrefix = er->ReadNullTerminatedString();

		er->Skip(120); // unknown

		for (u32 i = 0; i < binData.folderCount; i++)
		{
			BinIndexFolder folder = {};

			folder.nameHash = er->ReadU32();

			folder.nextNeighbourFolder = er->ReadS64();
			folder.previousFolder = er->ReadS64();

			folder.unknown = er->ReadU32();

			folder.nameOffset = er->ReadS64();

			if (folder.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - folder.nameOffset), SeekDirection::End);
				folder.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			folder.nextLowerFolder = er->ReadS64();
			folder.nextFile = er->ReadS64();

			binData.folders.push_back(folder);
		}

		for (u32 i = 0; i < binData.fileCount; i++)
		{
			BinIndexFile file = {};

			file.nameHash = er->ReadU32();
			file.nextFile = er->ReadS64();
			file.previousFolder = er->ReadS64();
			file.unknown = er->ReadU32();
			file.nameOffset = er->ReadS64();

			if (file.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - file.nameOffset), SeekDirection::End);
				file.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			file.offset = er->ReadS64();
			file.size = er->ReadS64();
			file.fileDataHash = er->ReadU32();
			file.writeTime = er->ReadU64();

			binData.files.push_back(file);
		}
	}

	void RMDPArchive::ReadBinHeaderV9(EndianReader* er)
	{
		binData.folderCount = er->ReadU32();
		binData.fileCount = er->ReadU32();

		binData.folders.reserve(binData.folderCount);
		binData.files.reserve(binData.fileCount);

		er->Skip(8); //unknown

		binData.nameContainerSize = er->ReadU32();

		er->Skip(128); //unknown

		for (u32 i = 0; i < binData.folderCount; i++)
		{
			BinIndexFolder folder = {};

			folder.nameHash = er->ReadU32();

			folder.nextNeighbourFolder = er->ReadS64();
			folder.previousFolder = er->ReadS64();

			folder.unknown = er->ReadU32();

			folder.nameOffset = er->ReadS64();

			if (folder.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - folder.nameOffset), SeekDirection::End);
				folder.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			folder.nextLowerFolder = er->ReadS64();
			folder.nextFile = er->ReadS64();

			binData.folders.push_back(folder);
		}

		for (u32 i = 0; i < binData.fileCount; i++)
		{
			BinIndexFile file = {};

			file.nameHash = er->ReadU32();
			file.nextFile = er->ReadS64();
			file.previousFolder = er->ReadS64();
			file.unknown = er->ReadU32();
			file.nameOffset = er->ReadS64();

			if (file.nameOffset != -1)
			{
				u32 position = er->GetPosition();
				er->Seek(-1 * (binData.nameContainerSize - file.nameOffset), SeekDirection::End);
				file.name = er->ReadNullTerminatedString();
				er->Seek(position, SeekDirection::Begin);
			}

			file.offset = er->ReadS64();
			file.size = er->ReadS64();
			file.fileDataHash = er->ReadU32();
			file.writeTime = er->ReadU64();

			binData.files.push_back(file);
		}
	}

	void RMDPArchive::ReadPackmetaFile()
	{
		packmetaData = {};

		EndianReader er(path.substr(0, path.find_last_of('.')).append(".packmeta"));

		packmetaData.pairCount = er.ReadU32();
		packmetaData.version = er.ReadU32();

		if (packmetaData.version != 1)
		{
			throw "unknown packmeta version";
		}

		packmetaData.fileCount = er.ReadU32();

		packmetaData.fileEntries.reserve(packmetaData.fileCount);

		packmetaData.pathContainerSize = er.ReadU32();

		for (u32 i = 0; i < packmetaData.fileCount; i++)
		{
			PackmetaFileEntry entry{};

			entry.path = er.ReadNullTerminatedString();

			packmetaData.fileEntries.push_back(entry);
		}

		for (u32 i = 0; i < packmetaData.fileCount; i++)
		{
			packmetaData.fileEntries[i].offset = er.ReadU32();
		}

		packmetaData.ridCount = er.ReadU32();

		packmetaData.ridEntries.reserve(packmetaData.ridCount);

		for (u32 i = 0; i < packmetaData.ridCount; i++)
		{
			PackmetaRIDEntry entry{};

			entry.rid = er.ReadU64();

			packmetaData.ridEntries.push_back(entry);
		}

		for (u32 i = 0; i < packmetaData.ridCount; i++)
		{
			packmetaData.ridEntries[i].offset = er.ReadU32();
		}

		for (const auto& fileEntry : packmetaData.fileEntries)
		{
			bool found = false;
			for (const auto& ridEntry : packmetaData.ridEntries)
			{
				if (ridEntry.offset == fileEntry.offset)
				{
					pathMap.insert(std::make_pair(fileEntry.path, ridEntry.rid));
					found = true;
					break;
				}
			}
			if(!found)
			{
				throw "not found";
			}
		}
	}
	string RMDPArchive::GetFilePath(BinIndexFile* file)
	{
		string filePath = file->name;

		BinIndexFolder* folder = &binData.folders[file->previousFolder];

		while (true)
		{
			if (!folder->name.empty())
			{
				filePath = folder->name + "\\" + filePath;
			}

			if (folder->previousFolder == -1)
			{
				break;
			}

			folder = &binData.folders[folder->previousFolder];
		}

		return filePath;
	}
	void RMDPArchive::ExportFile(string path, BinIndexFile* file)
	{
		EndianReader er(this->path);
		std::ofstream fileStream(path, std::ios_base::binary | std::ios::trunc);
		er.Seek(file->offset, SeekDirection::Begin);
		std::vector<u8> bytes = er.ReadBytes(file->size);
		fileStream.write((char*)bytes.data(), bytes.size());
		fileStream.close();
	}
	void RMDPArchive::ExportFolder(string path, BinIndexFolder* folder)
	{
		std::filesystem::path basePath(path);
		std::filesystem::path folderPath(path + "\\" + folder->name);
		if (!std::filesystem::exists(folderPath))
		{
			std::filesystem::create_directory(folderPath);
		}

		if (folder->nextFile != -1)
		{
			BinIndexFile* currentFile = &binData.files[folder->nextFile];

			while (true)
			{
				this->ExportFile(std::filesystem::path(folderPath.string() + "\\" + currentFile->name).string(), currentFile);

				if (currentFile->nextFile == -1)
					break;

				currentFile = &binData.files[currentFile->nextFile];
			}
		}

		if (folder->nextLowerFolder != -1)
		{
			BinIndexFolder* currentFolder = &binData.folders[folder->nextLowerFolder];

			while (true)
			{
				this->ExportFolder(std::filesystem::path(folderPath.string()).string(), currentFolder);

				if (currentFolder->nextNeighbourFolder == -1)
					break;

				currentFolder = &binData.folders[currentFolder->nextNeighbourFolder];
			}
		}
	}
}