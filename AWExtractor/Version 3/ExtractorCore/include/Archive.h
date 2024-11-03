#pragma once
#include "ReadableFile.h"

namespace ExtractorCore
{
	class Archive : public ReadableFile
	{
	public:
		Archive(string path);

		virtual void Draw() = 0;
	};
}