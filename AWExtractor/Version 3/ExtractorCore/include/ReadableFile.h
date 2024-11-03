#pragma once
#include "Common.h"
#include <EndianReader.h>

namespace ExtractorCore
{
	class ReadableFile
	{
	public:
		ReadableFile(string path);

	protected:
		EndianReader er;

	};
}