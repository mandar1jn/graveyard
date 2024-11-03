#include "TexturePreview.h"
#include "imgui.h"
#include <glad/glad.h>

namespace AWExtractor
{
	TexturePreview::TexturePreview(BinIndexFile* file) : PreviewFile(file)
	{
		texId = 0;

		glGenTextures(1, &texId);
	}
	TexturePreview::~TexturePreview()
	{
		glDeleteTextures(1, &texId);
	}
	void TexturePreview::Draw()
	{
		ImGui::Text("test");
		ImGui::Image(&texId, ImVec2(200, 200));
	}
}