#pragma once
#include "PreviewFile.h"
#include <map>

namespace AWExtractor
{
    class StringTablePreview : public PreviewFile
    {
    public:
        StringTablePreview(BinIndexFile* file);

        void Draw() override;

        std::map<string, string> strings;
    };

}