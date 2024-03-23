using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AWExtractor.Extensions
{
    public static class StringReaderExtension
    {
        public static string ReadNullTerminatedString(this BinaryReader br)
        {
            StringBuilder sb = new StringBuilder();

            while (true)
            {
                byte character = br.ReadByte();
                if (character == 0)
                {
                    break;
                }
                sb.Append(Encoding.ASCII.GetString(new byte[] { character }));
            }

            return sb.ToString();
        }

        public static string ReadStringWithSize(this BinaryReader br, uint size)
        {
            StringBuilder sb = new();

            for (int i = 0; i < size; i++)
            {
                sb.Append(Encoding.ASCII.GetString(new byte[] { br.ReadByte() }));
            }

            return sb.ToString();
        }

        public static uint ReadUint32BE(this BinaryReader br)
        {
            byte[] bytes = br.ReadBytes(4).Reverse().ToArray();
            return BitConverter.ToUInt32(bytes);
        }

        public static void Skip(this BinaryReader br, uint count)
        {
            br.BaseStream.Position += count;
        }

        public static void Skip(this BinaryReader br, long count)
        {
            br.BaseStream.Position += count;
        }
    }
}
