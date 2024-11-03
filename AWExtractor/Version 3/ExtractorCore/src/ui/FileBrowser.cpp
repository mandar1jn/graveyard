#include "ui/FileBrowser.h"
#include "imgui.h"

namespace ExtractorCore::UI
{
	std::vector<std::shared_ptr<Archive>> FileBrowser::archives = std::vector<std::shared_ptr<Archive>>();

	void FileBrowser::Draw()
	{
		if (ImGui::Begin("File Browser"))
		{
			for (u32 i = 0; i < archives.size(); i++)
			{
				ImGui::PushID(archives[i].get());
				archives[i]->Draw();
				ImGui::PopID();
			}
		}

		ImGui::End();
	}
}