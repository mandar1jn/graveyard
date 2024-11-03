#include "ReadableFile.h"

namespace ExtractorCore
{
	ReadableFile::ReadableFile(string path) : er(path, Endianness::Little)
	{
	}
}