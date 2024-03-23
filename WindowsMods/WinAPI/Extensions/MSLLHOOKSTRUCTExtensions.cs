using Windows.Win32.UI.WindowsAndMessaging;

namespace WindowsMods.WinAPI.Extensions
{
    public static class MSLLHOOKSTRUCTExtensions
    {
        public static bool WasWheelMovedUp(this MSLLHOOKSTRUCT hookStruct)
        {
            return (short)(hookStruct.mouseData >> 16) > 0;
        }

        public static bool WasWheelMovedDown(this MSLLHOOKSTRUCT hookStruct)
        {
            return (short)(hookStruct.mouseData >> 16) < 0;
        }
    }
}
