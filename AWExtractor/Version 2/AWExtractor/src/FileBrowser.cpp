#include "FileBrowser.h"
#include <imgui.h>
#include <ResourceManager.h>
#include <filesystem>
#include "AssetViewer.h"
#include <iostream>
#include <format>
#include "utils/PortableFileDialogs.h"

namespace AWExtractor
{
	void FileBrowser::AddFolderToTree(const RMDPArchive* archive, s64 index)
	{
		BinIndexFolder* folder = (BinIndexFolder*)&archive->binData.folders.at(index);

		if (ImGui::TreeNode(folder->name.c_str()))
		{
			if (folder->nextLowerFolder != -1)
			{
				AddFolderToTree(archive, folder->nextLowerFolder);
			}

			if (folder->nextFile != -1)
			{
				AWExtractor::BinIndexFile* file = (BinIndexFile*)&archive->binData.files.at(folder->nextFile);

				while (true)
				{
					ImGui::TreeNodeEx(file->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

					static BinIndexFile* currentPopupFile = nullptr;

					if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						AssetViewer::Instance->SetPreviewAsset(file);
					}
					else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup("File export popup");
						currentPopupFile = file;
					}
					if (ImGui::IsPopupOpen("File export popup") && currentPopupFile == file)
					{
						if (ImGui::BeginPopup("File export popup"))
						{
							if (ImGui::Button("Export file"))
							{
								string path = pfd::save_file(file->name).result();
								if (path != "")
								{
									RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);
									archive->ExportFile(path, file);
									ImGui::CloseCurrentPopup();
								}
							}
							ImGui::EndPopup();
						}
					}

					if (file->nextFile == -1)
						break;

					file = (BinIndexFile*)&archive->binData.files[file->nextFile];
				}
			}

			ImGui::TreePop();
		}
		static BinIndexFolder* currentPopupFolder = nullptr;
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Folder export popup");
			currentPopupFolder = folder;
		}
		if (ImGui::IsPopupOpen("Folder export popup") && currentPopupFolder == folder)
		{
			if (ImGui::BeginPopup("Folder export popup"))
			{
				if (ImGui::Button("Export folder"))
				{
					string path = pfd::select_folder("Select parent folder").result();
					if (path != "")
					{
						RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFolder(folder);
						archive->ExportFolder(path, folder);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}

		if (folder->nextNeighbourFolder != -1)
		{
			AddFolderToTree(archive, folder->nextNeighbourFolder);
		}
	}

	void FileBrowser::Draw()
	{
		ResourceManager* manager = ResourceManager::Instance;

		ImGui::Begin("File browser");
		
		for (const auto& archive : manager->rmdpArchives)
		{
			if (ImGui::TreeNode(std::filesystem::path(archive.path).filename().string().c_str()))
			{
				if (archive.binData.folderCount > 1)
				{
					FileBrowser::AddFolderToTree(&archive, 1);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
		
	}
}