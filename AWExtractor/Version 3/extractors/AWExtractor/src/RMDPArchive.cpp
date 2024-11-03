#include "RMDPArchive.h"
#include "imgui.h"
#include <filesystem>
#include "ui/FilePreviewWindow.h"
#include "ui/previews/StringPreview.h"
#include "previews/StringTablePreview.h"
#include "previews/ImagePreview.h"

namespace AWExtractor
{
	RMDPArchiveIndex::RMDPArchiveIndex(string path) : ExtractorCore::ReadableFile(path)
	{

		bigEndian = er.ReadBool();

		er.SetEndianness(bigEndian ? ExtractorCore::Endianness::Big : ExtractorCore::Endianness::Little);

		version = er.ReadU32();

		if (version != 2 && version != 7 && version != 8 && version != 9)
		{
			throw "Unsupported file version";
		}

		folderCount = er.ReadU32();
		fileCount = er.ReadU32();

		if (version >= 7)
		{
			er.Skip(8); // unknown
		}

		nameContainerSize = er.ReadU32();

		if (version != 9)
		{
			pathPrefix = er.ReadNullTerminatedString();
		}

		er.Skip((version != 9) ? 120 : 128); // unknown

		if (version == 2)
		{
			remastered = er.GetSize() - (std::streampos)nameContainerSize - er.GetPosition() != folderCount * 28 + fileCount * 40;
		}

		folders.reserve(folderCount);
		for (u32 i = 0; i < folderCount; i++)
		{
			FolderEntry folder = {};

			folder.nameHash = er.ReadU32();

			if (remastered)
			{
				er.Skip(4); // unknown
			}

			if (version >= 8 || remastered)
			{
				folder.nextNeighbourFolder = er.ReadS64();
				folder.previousFolder = er.ReadS64();
			}
			else
			{
				folder.nextNeighbourFolder = er.ReadS32();
				folder.previousFolder = er.ReadS32();
			}

			er.Skip(4); // unknown
			if (remastered)
			{
				er.Skip(4); // unknown
			}

			if (version >= 8 || remastered)
			{
				folder.nameOffset = er.ReadS64();
				folder.nextLowerFolder = er.ReadS64();
				folder.nextFile = er.ReadS64();
			}
			else
			{
				folder.nameOffset = er.ReadS32();
				folder.nextLowerFolder = er.ReadS32();
				folder.nextFile = er.ReadS32();
			}

			folders.push_back(folder);
		}
	
		files.reserve(fileCount);
		for (u32 i = 0; i < fileCount; i++)
		{
			FileEntry file = {};

			file.nameHash = er.ReadU32();

			if (remastered)
			{
				er.Skip(4); // unknown
			}

			if (version >= 8 || remastered)
			{
				file.nextFile = er.ReadS64();
				file.previousFolder = er.ReadS64();
			}
			else
			{
				file.nextFile = er.ReadS32();
				file.previousFolder = er.ReadS32();
			}

			er.Skip(4); // unknown
			if (remastered)
			{
				er.Skip(4); // unknown
			}

			if (version >= 8 || remastered)
			{
				file.nameOffset = er.ReadS64();
			}
			else
			{
				file.nameOffset = er.ReadS32();
			}

			file.offset = er.ReadS64();
			file.size = er.ReadS64();
			file.fileDataHash = er.ReadU32();

			if (version >= 7)
			{
				file.writeTime = er.ReadU64();
			}
			else if (remastered)
			{
				er.Skip(4); // unknown
			}

			files.push_back(file);
		}

		for (u32 i = 0; i < folderCount; i++)
		{
			if (folders[i].nameOffset != -1)
			{
				er.Seek(-1 * (nameContainerSize - folders[i].nameOffset), ExtractorCore::SeekDirection::End);
				std::streampos pos = er.GetPosition();
				folders[i].name = er.ReadNullTerminatedString();
			}
		}

		for (u32 i = 0; i < fileCount; i++)
		{
			if (files[i].nameOffset == -1)
			{
				throw "unsupported";
			}

			er.Seek(-1 * (nameContainerSize - files[i].nameOffset), ExtractorCore::SeekDirection::End);
			std::streampos pos = er.GetPosition();
			files[i].name = er.ReadNullTerminatedString();
		}
	}

	RMDPArchive::RMDPArchive(string path) : ExtractorCore::Archive(path), index(path.substr(0, path.find("rmdp")).append("bin"))
	{
		fileName = std::filesystem::path(path).filename().string();
	}
	void RMDPArchive::Draw()
	{
		if(ImGui::TreeNode(fileName.c_str()))
		{
			DrawFolder(0);

			ImGui::TreePop();
		}
	}
	void RMDPArchive::DrawFolder(s64 folderIndex)
	{
		FolderEntry* folder = &index.folders[folderIndex];

		bool open = false;
		bool skipped = false;

		
		if (folderIndex == 0 && !index.pathPrefix.empty() && folder->name.empty()) [[unlikely]]
		{
			open = ImGui::TreeNode(index.pathPrefix.c_str());
		}
		else if (folder->name.empty()) [[unlikely]]
		{
			open = true;
			skipped = true;
		}
		else [[likely]]
		{
			open = ImGui::TreeNode(folder->name.c_str());
		}

		if (open)
		{
			if (folder->nextLowerFolder != -1)
			{
				DrawFolder(folder->nextLowerFolder);
			}

			if (folder->nextFile != -1)
			{
				FileEntry* file = &index.files[folder->nextFile];

				while (true)
				{
					ImGui::TreeNodeEx(file->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

					if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						this->er.Seek(file->offset, ExtractorCore::SeekDirection::Begin);
						std::vector<u8> bytes = this->er.ReadBytes(file->size);

						if (file->name == "string_table.bin")
						{
							ExtractorCore::UI::FilePreviewWindow::previews.push_back(std::make_shared<AWExtractor::UI::StringTablePreview>(file->name, std::make_shared<ExtractorCore::EndianReader>(bytes)));
						}
						else if (file->name.ends_with(".tex"))
						{
							ExtractorCore::UI::FilePreviewWindow::previews.push_back(std::make_shared<AWExtractor::UI::ImagePreview>(file->name, std::make_shared<ExtractorCore::EndianReader>(bytes)));
						}
						else
						{
							ExtractorCore::UI::FilePreviewWindow::previews.push_back(std::make_shared<ExtractorCore::UI::Previews::StringPreview>(file->name, std::make_shared<ExtractorCore::EndianReader>(bytes)));
						}
					}

					if (file->nextFile == -1)
					{
						break;
					}

					file = &index.files[file->nextFile];
				}
			}

			if (!skipped)
			{
				ImGui::TreePop();
			}
		}

		if (folder->nextNeighbourFolder != -1)
		{
			DrawFolder(folder->nextNeighbourFolder);
		}
	}
}