#include "ui/previews/StringPreview.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace ExtractorCore::UI::Previews
{
	StringPreview::StringPreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er) : FilePreview(fileName, er)
	{
		std::streampos size = er->GetSize();
		std::vector<u8> data = er->ReadBytes(size);
		stringData = string(data.begin(), data.end());
	}

	void StringPreview::Draw()
	{
		ImGuiContext* ctx = ImGui::GetCurrentContext();

		const bool need_backup = (ctx->CurrentWindow->DC.TextWrapPos < 0.0f);
		if (need_backup)
		{
			ImGui::PushTextWrapPos(0.0f);
		}
		ImGui::TextUnformatted(stringData.c_str());
		if (need_backup)
		{
			ImGui::PopTextWrapPos();
		}
		
	}
}
