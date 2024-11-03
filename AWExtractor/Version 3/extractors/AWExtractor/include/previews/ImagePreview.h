#pragma once
#include "Common.h"

#include "ui/FilePreviewWindow.h"
#include "glad/glad.h"

namespace AWExtractor::UI
{

	class ImagePreview : public ExtractorCore::UI::FilePreview
	{
	public:
		ImagePreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er);

		void Draw() override;

		~ImagePreview();

	private:
		GLuint texID = 0;
		GLint width = 0;
		GLint height = 0;
	};

}