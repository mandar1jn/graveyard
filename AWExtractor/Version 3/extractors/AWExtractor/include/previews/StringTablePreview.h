#pragma once
#include "Common.h"

#include "ui/FilePreviewWindow.h"
#include <EndianReader.h>
#include <unordered_map>

namespace AWExtractor::UI
{

	class StringTablePreview : public ExtractorCore::UI::FilePreview
	{
	public:
		StringTablePreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er);

		void Draw() override;

	private:
		u32 count = 0;
		std::vector<std::pair<string, string>> strings = std::vector<std::pair<string, string>>();
	};

}