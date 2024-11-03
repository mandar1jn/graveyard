#include "MenuBar.h"
#include "imgui.h"
#include <ResourceManager.h>
#include "utils/PortableFileDialogs.h"
#include <filesystem>

namespace AWExtractor
{

	void MenuBar::Draw()
	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
				auto selection = pfd::open_file("Select a file", "",
					{ "RMDP files", "*.rmdp" }, pfd::opt::multiselect).result();

				if (!selection.empty())
				{
					for (auto& path : selection)
					{
						ResourceManager::Instance->LoadRMDPArchive(path);
					}
				}
			}

			if (ImGui::MenuItem("Open Folder"))
			{
				auto selection = pfd::select_folder("Select data folder").result();

				if (!selection.empty())
				{
					for (const auto& entry : std::filesystem::directory_iterator(selection))
					{
						if (entry.path().extension() == ".rmdp")
						{
							ResourceManager::Instance->LoadRMDPArchive(entry.path().string());
						}
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}