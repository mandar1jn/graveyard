#pragma once
#include "files/RMDPArchive.h"

namespace AWExtractor
{

	class FileBrowser
	{
	public:
		static void AddFolderToTree(const RMDPArchive* archive, s64 index);

		static void Draw();
	};

}