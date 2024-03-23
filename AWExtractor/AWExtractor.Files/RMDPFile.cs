using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AWExtractor.Files.BinFile;
using AWExtractor.Utils;

namespace AWExtractor.Files
{
    public class RMDPFile
    {
        public BinFile.BinFile binFile;

        public RMDPFile(string path)
        {
            string binPath = Path.ChangeExtension(path, "bin");

            binFile = new(binPath);

            /*BinaryReader br = new(File.Open(path, FileMode.Open));

            var pathElements = path.Split('\\').Take(path.Split('\\').Length - 1);
            string basePath = string.Join("\\", pathElements);

            foreach(BinFileEntry file in binFile.fileEntries)
            {

                string filePath = file.name;

                string folderPath = "";

                long prevFolder = file.prevFolder;
                while (prevFolder > 1)
                {
                    folderPath = $"{binFile.folderEntries[(int)prevFolder].name}\\" + folderPath;
                    prevFolder = binFile.folderEntries[(int)prevFolder].prevFolder;
                }

                folderPath = Path.Combine(basePath, folderPath);

                if(!Directory.Exists(folderPath))
                {
                    Directory.CreateDirectory(folderPath);
                }

                string fullPath = Path.Combine(folderPath, filePath);

                br.BaseStream.Seek((long)file.offset, SeekOrigin.Begin);
                byte[] data = br.ReadBytes((int)file.size);

                uint crc = Crypto.ComputeCRC32(data);

                if (crc != file.fileDataHash)
                    throw new Exception("Computed file hash did not match provided hash");



                BinaryWriter bw = new(File.Open(fullPath, FileMode.Create));
                bw.Write(data);
                bw.Close();
                bw.Dispose();
            }*/
        }

    }
}
