#include "EndianReader.h"
#include <sstream>

namespace ExtractorCore
{
	EndianReader::EndianReader(std::vector<u8> data, Endianness endianness)
	{
		string str(data.begin(), data.end());
		stream = std::make_unique<std::istringstream>(str);
		this->endianness = endianness;
	}

	EndianReader::EndianReader(std::string filePath, Endianness endianness)
	{
		stream = std::make_unique<std::ifstream>(filePath, std::ios_base::binary | std::ios_base::in);
		this->endianness = endianness;
	}

	void EndianReader::SetEndianness(Endianness endianness)
	{
		this->endianness = endianness;
	}

	void EndianReader::Seek(s64 position, SeekDirection direction)
	{
		stream->seekg(position, direction);
	}

	void EndianReader::Skip(s32 skipAmount)
	{
		std::streampos pos = GetPosition();

		pos += skipAmount;

		this->Seek(pos, SeekDirection::Begin);
	}

	std::streampos EndianReader::GetPosition()
	{
		return stream->tellg();
	}

	std::streampos EndianReader::GetSize()
	{
		std::streampos startPos = GetPosition();
		Seek(0, SeekDirection::End);
		std::streampos size = GetPosition();
		Seek(startPos, SeekDirection::Begin);
		return size;
	}

	bool EndianReader::ReadBool()
	{
		return stream->get() > 0;
	}

	u8 EndianReader::ReadU8()
	{
		u8 value = 0;
		stream->read((char*)&value, 1);
		return value;
	}

	u16 EndianReader::ReadU16()
	{
		u16 value = 0;
		stream->read((char*)&value, 2);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_ushort(value);
		}

		return value;
	}

	u32 EndianReader::ReadU32()
	{
		u32 value = 0;
		stream->read((char*)&value, 4);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_ulong(value);
		}

		return value;
	}

	s32 EndianReader::ReadS32()
	{
		s32 value = 0;
		stream->read((char*)&value, 4);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_ulong(value);
		}

		return value;
	}

	u64 EndianReader::ReadU64()
	{
		u64 value = 0;
		stream->read((char*)&value, 8);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_uint64(value);
		}

		return value;
	}

	s64 EndianReader::ReadS64()
	{
		s64 value = 0;
		stream->read((char*)&value, 8);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_uint64(value);
		}

		return value;
	}

	float EndianReader::ReadFloat()
	{
		float value = 0;
		stream->read((char*)&value, 4);

		if (endianness == Endianness::Big)
		{
			value = _byteswap_ulong(value);
		}

		return value;
	}

	std::vector<u8> EndianReader::ReadBytes(s64 count)
	{
		std::vector<uint8_t> bytes = std::vector<u8>();
		bytes.resize(count);
		stream->read((char*)bytes.data(), count);
		return bytes;
	}

	std::string EndianReader::ReadNullTerminatedString()
	{
		char character = 0;
		std::string result = "";

		while (true)
		{
			character = stream->get();

			if (character == 0)
				break;

			result += character;
		}
		return result;
	}

	std::string EndianReader::ReadString()
	{
		u32 length = ReadU32();
		return ReadString(length);
	}

	std::string EndianReader::ReadString(u32 length)
	{
		std::string result;

		for (u32 i = 0; i < length; i++)
		{
			char character = stream->get();
			result += character;
		}
		return result;
	}

	std::string EndianReader::ReadWideString()
	{
		u32 length = ReadU32();
		return ReadWideString(length);
	}

	std::string EndianReader::ReadWideString(u32 length)
	{
		std::string result;

		for (u32 i = 0; i < length; i++)
		{
			u16 character = ReadU16();
			result += character;
		}
		return result;
	}
}