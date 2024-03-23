using AWExtractor.Extensions;
using AWExtractor.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AWExtractor.Files.BinFile
{
    public class BinFileEntry
    {
        public uint nameHash;
        public long nextFile;
        public long prevFolder;
        public uint flags;
        public string name;
        public ulong offset;
        public ulong size;
        public uint fileDataHash;
        public ulong writeTime;

        public BinFileEntry(BinaryReader br, uint nameSize)
        {
            nameHash = br.ReadUInt32();
            nextFile = br.ReadInt64();
            prevFolder = br.ReadInt64();
            flags = br.ReadUInt32();

            long nameOffset = br.ReadInt64();
            {
                if (nameOffset != -1)
                {
                    long startPos = br.BaseStream.Position;
                    br.BaseStream.Seek(nameOffset - nameSize, SeekOrigin.End);
                    name = br.ReadNullTerminatedString();
                    br.BaseStream.Seek(startPos, SeekOrigin.Begin);
                }
                else
                {
                    name = "";
                }
            }
            if (Crypto.ComputeCRC32(name.ToLower()) != nameHash)
                throw new Exception("Warning: nameHash did not match name");

            offset = br.ReadUInt64();
            size = br.ReadUInt64();

            fileDataHash = br.ReadUInt32();
            writeTime = br.ReadUInt64();
        }

    }
}
