#pragma once
#include "DrawableComponent.h"
#include <functional>
#include <Common.h>
#include "imgui.h"

namespace ExtractorCore::UI
{
	class MenuBarMenu;
	class MenuBarItem
	{
		friend class MenuBarMenu;

	public:
		MenuBarItem(string name, std::function<void()> callback);

		string name;

	private:

		std::function<void()> callback;
	};
	class MenuBarMenu
	{
	public:
		MenuBarMenu(string name);

		void Draw();

		void AddMenu(MenuBarMenu menu);
		void AddItem(MenuBarItem item);

		string name;
		std::vector<MenuBarMenu> subMenus;
		std::vector<MenuBarItem> items;
	};
	class MenuBar : public DrawableComponent
	{
	public:
		static void Draw();

		static void AddMenu(MenuBarMenu menu);

	private:
		static std::vector<MenuBarMenu> menus;
	};
}