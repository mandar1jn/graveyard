#include "ui/MenuBar.h"
#include "ui/Aboutwindow.h"
#include "imgui.h"

namespace ExtractorCore::UI
{
	std::vector<MenuBarMenu> MenuBar::menus = std::vector<MenuBarMenu>();

	void MenuBar::Draw()
	{
		ImGui::BeginMainMenuBar();

		for (u32 i = 0; i < menus.size(); i++)
		{
			menus[i].Draw();
		}

		if (ImGui::MenuItem("About"))
		{
			AboutWindow::shouldDraw = true;
		}

		ImGui::EndMainMenuBar();
	}
	void MenuBar::AddMenu(MenuBarMenu menu)
	{
		menus.push_back(menu);
	}

	MenuBarItem::MenuBarItem(string name, std::function<void()> callback)
	{
		this->name = name;
		this->callback = callback;
	}

	MenuBarMenu::MenuBarMenu(string name)
	{
		this->name = name;
		subMenus = std::vector<MenuBarMenu>();
		items = std::vector<MenuBarItem>();
	}
	void MenuBarMenu::AddMenu(MenuBarMenu menu)
	{
		subMenus.push_back(menu);
	}
	void MenuBarMenu::AddItem(MenuBarItem item)
	{
		items.push_back(item);
	}
	void MenuBarMenu::Draw()
	{
		if (ImGui::BeginMenu(name.c_str()))
		{

			for (u32 i = 0; i < subMenus.size(); i++)
			{
				subMenus[i].Draw();
			}

			for (u32 i = 0; i < items.size(); i++)
			{
				if (ImGui::MenuItem(items[i].name.c_str()))
				{
					items[i].callback();
				}
			}

			ImGui::EndMenu();
		}
	}
}