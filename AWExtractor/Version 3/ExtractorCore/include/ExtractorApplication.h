#pragma once
#include "Common.h"
#include <vector>
#include "ui/DrawableComponent.h"

namespace ExtractorCore
{
	class ExtractorApplication
	{
	public:
		ExtractorApplication(string applicationName);

		int Run(char argc, char** argv);

		void AddDrawableComponent(UI::DrawableComponent* component);

	private:
		string applicationName;

		std::vector<UI::DrawableComponent*> drawableComponents;
	};
}