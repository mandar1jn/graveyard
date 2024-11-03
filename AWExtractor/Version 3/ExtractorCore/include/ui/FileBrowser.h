#pragma once
#include "DrawableComponent.h"

#include <functional>
#include <Archive.h>

namespace ExtractorCore::UI
{
	class FileBrowser 
	{
	public:
		static void Draw();

		static std::vector<std::shared_ptr<Archive>> archives;
		
	};
}