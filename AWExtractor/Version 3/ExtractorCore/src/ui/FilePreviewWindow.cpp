#include "ui/FilePreviewWindow.h"
#include "imgui.h"

namespace ExtractorCore::UI
{

	std::vector<std::shared_ptr<FilePreview>> FilePreviewWindow::previews = std::vector<std::shared_ptr<FilePreview>>();
	std::vector<u8> open = std::vector<u8>();

	void FilePreviewWindow::Draw()
	{

		if (ImGui::Begin("Previews"))
		{

			if (ImGui::BeginTabBar("PreviewTabBar", ImGuiTabBarFlags_Reorderable))
			{

				if (previews.size() != 0)
				{
					open.clear();

					open.resize(previews.size(), true);

					for (u32 i = 0; i < previews.size(); i++)
					{
						ImGui::PushID(previews[i].get());
						if (ImGui::BeginTabItem(previews[i]->fileName.c_str(), (bool*)&open[i]))
						{
							previews[i]->Draw();
							ImGui::EndTabItem();
						}
						ImGui::PopID();
					}

					for (s64 i = previews.size() - 1; i >= 0; --i)
					{
						if ((bool)open[i] != true)
						{
							previews.erase(previews.begin() + i);
						}
					}
				}

				ImGui::EndTabBar();
			}
		}

		ImGui::End();
	}

}