#pragma once
#include "PreviewFile.h"

namespace AWExtractor
{
	class StringPreview : public PreviewFile
	{
	public:
		StringPreview(BinIndexFile* file);

		void Draw() override;

	private:
		string metaString = "";

	};

}