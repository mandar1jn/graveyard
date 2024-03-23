using AWExtractor.Extensions;
using AWExtractor.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AWExtractor.Files.BinFile
{
    public class BinFolderEntry
    {

        public uint nameHash;
        public long nextNeighbourfolder;
        public long prevFolder;
        public string name;
        public long nextLowerFolder;
        public long nextFile;

        public BinFolderEntry(BinaryReader br, uint nameSize)
        {
            nameHash = br.ReadUInt32();
            nextNeighbourfolder = br.ReadInt64();
            prevFolder = br.ReadInt64();

            br.Skip(4);

            long nameOffset = br.ReadInt64();

            {
                if(nameOffset != -1)
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
                throw new Exception("nameHash did not match name");

            nextLowerFolder = br.ReadInt64();
            nextFile = br.ReadInt64();
        }

    }
}
