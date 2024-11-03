#include "StringPreview.h"

namespace AWExtractor
{
	StringPreview::StringPreview(BinIndexFile* file) : PreviewFile(file)
	{

		metaString = er->ReadString(file->size);

	}

	void StringPreview::Draw()
	{
		ImGui::TextUnformatted(metaString.c_str());
	}
}