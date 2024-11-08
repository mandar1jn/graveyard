#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <common.h>

enum Endianness
{
	Big,
	Little
};

enum SeekDirection
{
	Begin,
	Current,
	End
};

class EndianReader
{
public:
	EndianReader(std::string filePath, Endianness endianness = Endianness::Little);

	void SetEndianness(Endianness endianness);
	void Seek(s64 position, SeekDirection direction);
	void Skip(s32 skipAmount);

	std::streampos GetPosition();

	bool ReadBool();

	u8 ReadU8();

	u16 ReadU16();

	u32 ReadU32();
	s32 ReadS32();

	u64 ReadU64();
	s64 ReadS64();

	float ReadFloat();

	std::vector<u8> ReadBytes(u32 count);

	std::string ReadNullTerminatedString();
	std::string ReadString();
	std::string ReadString(u32 length);

	std::string ReadWideString();
	std::string ReadWideString(u32 length);

	template<typename T> T ReadStruct()
	{
		return *(T*)ReadBytes(sizeof(T)).data();
	}

	std::ifstream stream;
	Endianness endianness;
};