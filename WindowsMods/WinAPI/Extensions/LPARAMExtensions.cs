using System.Runtime.InteropServices;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;

namespace WindowsMods.WinAPI.Extensions
{
    public static class LPARAMExtensions
    {
        public static MSLLHOOKSTRUCT ToMouseInfo(this LPARAM lParam)
        {
            return Marshal.PtrToStructure<MSLLHOOKSTRUCT>(lParam);
        }
    }
}
