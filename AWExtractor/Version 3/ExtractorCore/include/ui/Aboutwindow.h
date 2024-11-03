#pragma once
#include "Common.h"

namespace ExtractorCore::UI
{

	struct AboutInfo
	{
		string toolName = "";
	};

	class AboutWindow
	{
	public:
		static void Draw();

		static AboutInfo info;
		static bool shouldDraw;
	};

}