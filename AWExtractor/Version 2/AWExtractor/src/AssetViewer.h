#pragma once
#include "files/RMDPArchive.h"
#include <memory>
#include <filesystem>
#include <files/previews/PreviewFile.h>

namespace AWExtractor
{

	class AssetViewer
	{
	public:
		static void Draw();

		static void SetPreviewAsset(BinIndexFile* file);

		static AssetViewer* Instance;

	private:
		static BinIndexFile* currentPreview;
		static std::unique_ptr<PreviewFile> previewFile;

	};

}