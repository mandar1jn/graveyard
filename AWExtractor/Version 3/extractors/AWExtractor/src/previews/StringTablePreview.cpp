#include "previews/StringTablePreview.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace AWExtractor::UI
{
	StringTablePreview::StringTablePreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er) : ExtractorCore::UI::FilePreview(fileName, er)
	{
		count = er->ReadU32();

		for (u32 i = 0; i < count; i++)
		{
			strings.push_back({ er->ReadString(), er->ReadWideString() });
		}
	}

	void StringTablePreview::Draw()
	{

		for (u32 i = 0; i < count; i++)
		{
			ImGui::Text(strings[i].first.c_str());

			ImGuiContext* ctx = ImGui::GetCurrentContext();

			const bool need_backup = (ctx->CurrentWindow->DC.TextWrapPos < 0.0f);
			if (need_backup)
			{
				ImGui::PushTextWrapPos(0.0f);
			}
			ImGui::Indent(10);
			ImGui::TextUnformatted(strings[i].second.c_str());
			ImGui::Unindent(10);
			if (need_backup)
			{
				ImGui::PopTextWrapPos();
			}

			ImGui::NewLine();
		}

	}
}
