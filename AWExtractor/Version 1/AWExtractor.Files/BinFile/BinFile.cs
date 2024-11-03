using AWExtractor.Extensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AWExtractor.Files.BinFile
{
    public class BinFile
    {
        bool bigEndian;
        uint version;
        public List<BinFolderEntry> folderEntries;
        public List<BinFileEntry> fileEntries;

        ulong unknown1;
        byte[] unknown = new byte[120];

        public BinFile(string path)
        {
            BinaryReader br = new(File.Open(path, FileMode.Open));

            bigEndian = br.ReadBoolean();

            if(bigEndian)
            {
                throw new NotImplementedException("Big endian .bin files not supported yet");
            }

            version = br.ReadUInt32();

            switch(version)
            {
                case 9:
                    ReadHeaderV9(br);
                    break;
                case 8:
                    ReadHeaderV8(br);
                    break;
                default:
                    throw new NotImplementedException($".bin version {version} not implemented yet");
            }

            br.Close();
            br.Dispose();

        }

        void ReadHeaderV8(BinaryReader br)
        {
            folderEntries = new List<BinFolderEntry>((int)br.ReadUInt32());
            fileEntries = new List<BinFileEntry>((int)br.ReadUInt32());

            unknown1 = br.ReadUInt64();

            // total size of name storage
            uint nameSize = br.ReadUInt32();
            string pathPrefix = br.ReadNullTerminatedString();

            unknown = br.ReadBytes(120);

            for (int i = 0; i < folderEntries.Capacity; i++)
            {
                folderEntries.Add(new BinFolderEntry(br, nameSize));
            }

            for (int i = 0; i < fileEntries.Capacity; i++)
            {
                fileEntries.Add(new BinFileEntry(br, nameSize));
            }
        }

        void ReadHeaderV9(BinaryReader br)
        {
            folderEntries = new List<BinFolderEntry>((int)br.ReadUInt32());
            fileEntries = new List<BinFileEntry>((int)br.ReadUInt32());

            unknown1 = br.ReadUInt64();

            // total size of name storage
            uint nameSize = br.ReadUInt32();

            unknown = br.ReadBytes(128);

            for (int i = 0; i < folderEntries.Capacity; i++)
            {
                folderEntries.Add(new BinFolderEntry(br, nameSize));
            }

            for (int i = 0; i < fileEntries.Capacity; i++)
            {
                fileEntries.Add(new BinFileEntry(br, nameSize));
            }
        }
    }
}
