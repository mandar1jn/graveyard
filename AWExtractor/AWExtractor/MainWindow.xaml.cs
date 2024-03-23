using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AWExtractor.Files;
using AWExtractor.Files.BinFile;
using Microsoft.Win32;

namespace AWExtractor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void OpenFolder_Click(object sender, RoutedEventArgs e)
        {
            OpenFolderDialog dialog = new()
            {
                Multiselect = false,
                Title = "Select Quantum Break data folder"
            };

            bool? result = dialog.ShowDialog();

            if(result != true)
            {
                return;
            }

            string folderPath = dialog.FolderName;

            var files = Directory.EnumerateFiles(folderPath);
            if (!files.Where(file => file.EndsWith(".rmdp")).Any())
            {
                MessageBox.Show($"Folder \"{dialog.FolderName}\" contains no .rmdp files", "No .rmdp files found in the folder", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            List<RMDPFile> dataFiles = [];

            foreach (string file in files)
            {
                if(file.EndsWith(".rmdp"))
                {
                    dataFiles.Add(new RMDPFile(file));
                }
            }

            Files.Items.Clear();

            foreach (RMDPFile file in dataFiles)
            {
                if (file.binFile.folderEntries.Count > 1)
                    RecursivelyPopulateFileTree(file.binFile.folderEntries[1], file.binFile);
            }

        }

        (TreeViewItem, TreeViewItem?) GetSelf(BinFile file, BinFolderEntry folder)
        {
            ItemCollection items = Files.Items;
            TreeViewItem? current = null;
            TreeViewItem? last = null;

            List<string> components = GetPathComponents(file, folder);

            foreach (string component in components)
            {
                bool found = false;
                foreach (TreeViewItem item in items)
                {
                    if((string)item.Header == component)
                    {
                        last = current;
                        current = item;
                        items = current.Items;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    last = current;
                    current = new()
                    {
                        Header = component,
                    };
                    if (last != null)
                    {
                        last.Items.Add(current);
                    }
                    else
                    {
                        Files.Items.Add(current);
                    }
                }
            }

            return (current!, last);
        }

        List<string> GetPathComponents(BinFile file, BinFolderEntry folder)
        {
            List<string> components = [];

            components.Add(folder.name);

            long index = folder.prevFolder;
            while((ulong)index != 0xFFFFFFFFFFFFFFFF && index != 0)
            {
                folder = file.folderEntries[(int)index];

                components.Add(folder.name);

                index = folder.prevFolder;
            }

            components.Reverse();

            return components;
        }

        internal void RecursivelyPopulateFileTree(BinFolderEntry folder, BinFile binFile)
        {
            while(true)
            {
                (TreeViewItem, TreeViewItem?) items = GetSelf(binFile, folder);
                TreeViewItem dir = items.Item1;
                TreeViewItem? parent = items.Item2;

                long fileIndex = folder.nextFile;
                while((ulong)fileIndex != 0xFFFFFFFFFFFFFFFF)
                {
                    BinFileEntry file = binFile.fileEntries[(int)fileIndex];

                    dir.Items.Add(new TreeViewItem()
                    {
                        Header = file.name
                    });

                    fileIndex = file.nextFile;
                }

                if ((ulong)folder.nextLowerFolder != 0xFFFFFFFFFFFFFFFF)
                {
                    RecursivelyPopulateFileTree(binFile.folderEntries[(int)folder.nextLowerFolder], binFile);
                }

                if ((ulong)folder.nextNeighbourfolder == 0xFFFFFFFFFFFFFFFF)
                    break;

                folder = binFile.folderEntries[(int)folder.nextNeighbourfolder];
            }
        }
    }
}