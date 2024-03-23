using WindowsMods.Core;
using WindowsMods.UI;
using WindowsMods.WinAPI;

namespace WindowsMods
{
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();

            WinAPIController.ShowConsoleIfDebug();

            ModLoader.Instance.LoadMods();

            Application.Run(new TrayIcon());

            
        }
    }
}