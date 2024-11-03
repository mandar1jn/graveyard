#include "AssetViewer.h"
#include <imgui.h>
#include <ResourceManager.h>
#include <files/previews/TexturePreview.h>
#include <files/previews/StringPreview.h>
#include <files/previews/StringTablePreview.h>
#include <files/previews/ModelPreview.h>

namespace AWExtractor
{
	AssetViewer* AssetViewer::Instance;
	static bool loaded = false;
	BinIndexFile* AssetViewer::currentPreview;
	std::unique_ptr<PreviewFile> AssetViewer::previewFile = nullptr;

	void AssetViewer::Draw()
	{
		ImGui::Begin("Preview");

		if (previewFile != nullptr)
		{
			previewFile->Draw();
		}
		else
		{
			ImGui::Text("No supported asset selected");
		}

		ImGui::End();
	}
	void AssetViewer::SetPreviewAsset(BinIndexFile* file)
	{
		std::filesystem::path path(file->name);

		std::filesystem::path extension = path.extension();

		previewFile.release();

		if (extension == ".tex")
		{
			currentPreview = file;

			RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);

			if (archive != nullptr)
			{
				previewFile = std::make_unique<TexturePreview>(TexturePreview(file));
			}
		}
		else if(extension == ".meta" || extension == ".srt" || (extension == ".xml" && (path.string().find("xaitcontrol") == std::string::npos)) || extension == ".asset" || extension == ".txt" || extension == ".xsl" || extension == ".json" || extension == ".matdef")
		{
			currentPreview = file;

			RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);

			if (archive != nullptr)
			{
				previewFile = std::make_unique<StringPreview>(StringPreview(file));
			}
		}
		else if (path.filename() == "string_table.bin")
		{
			currentPreview = file;

			RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);

			if (archive != nullptr)
			{
				previewFile = std::make_unique<StringTablePreview>(StringTablePreview(file));
			}
		}
		else if (extension == ".binfbx")
		{
			currentPreview = file;

			RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);

			if (archive != nullptr)
			{
				previewFile = std::make_unique<ModelPreview>(ModelPreview(file));
			}
		}
		else
		{
			currentPreview = file;
			previewFile = nullptr;
		}
		
	}
}