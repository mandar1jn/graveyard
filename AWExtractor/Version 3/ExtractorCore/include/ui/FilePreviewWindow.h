#pragma once
#include "Common.h"

#include <vector>
#include <memory>
#include <EndianReader.h>

namespace ExtractorCore::UI
{

	class FilePreview
	{
	public:
		FilePreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er) : fileName(fileName), er(er) {}

		virtual void Draw() = 0;
		
		string fileName;
		std::shared_ptr<ExtractorCore::EndianReader> er;
	};

	class FilePreviewWindow
	{
	public:
		static void Draw();

		static std::vector<std::shared_ptr<FilePreview>> previews;
	};

}