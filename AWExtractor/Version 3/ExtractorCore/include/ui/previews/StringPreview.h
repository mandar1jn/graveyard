#pragma once

#include "ui/FilePreviewWindow.h"

namespace ExtractorCore::UI::Previews
{

	class StringPreview : public FilePreview
	{
	public:
		StringPreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er);

		void Draw() override;

	private:
		string stringData;
	};

}