#pragma once
#include "PreviewFile.h"
#include <glad/glad.h>

namespace AWExtractor
{

	class TexturePreview : public PreviewFile
	{
	public:
		TexturePreview(BinIndexFile* file);
		~TexturePreview();

		void Draw() override;

	private:
		GLuint texId;
	};

}