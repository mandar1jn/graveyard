#include "StringTablePreview.h"

namespace AWExtractor
{
	StringTablePreview::StringTablePreview(BinIndexFile* file) : PreviewFile(file)
	{
		strings = std::map<string, string>();

		u32 count = er->ReadU32();

		for (u32 i = 0; i < count; i++)
		{
			strings.insert({ er->ReadString(), er->ReadWideString() });
		}
	}
	void StringTablePreview::Draw()
	{
		for (auto const& [key, val] : strings)
		{
			ImGui::Text("%s: %s", key.c_str(), val.c_str());
		}
	}
}