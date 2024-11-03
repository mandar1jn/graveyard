#pragma once
#include <utils/EndianReader.h>
#include "imgui.h"
#include <files/RMDPArchive.h>
#include "ResourceManager.h"

namespace AWExtractor
{

	class PreviewFile
	{
	public:
		PreviewFile(BinIndexFile* file)
		{
			if (file != nullptr)
			{
				RMDPArchive* archive = ResourceManager::Instance->GetArchiveForFile(file);

				er = new EndianReader(archive->path);
				er->Seek(file->offset, SeekDirection::Begin);
			}
			else
			{
				er = nullptr;
			}
		}

		virtual void Draw() = 0;

	protected:
		EndianReader* er;
	};

}