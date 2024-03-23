using System.ComponentModel;
using System.Runtime.InteropServices;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;
using WindowsMods.WinAPI.Structures;
using Windows.Win32.UI.Input.KeyboardAndMouse;

namespace WindowsMods.WinAPI
{
    public static class WinAPIController
    {
        private static HWND consoleWindowHandle = PInvoke.GetConsoleWindow();
        public static bool consoleShown { get; private set; } = !consoleWindowHandle.IsNull;

        public static void ShowConsoleIfDebug()
        {
#if DEBUG
            if (!consoleShown)
            {
                ShowConsoleWindow();
            }
#endif
        }

        public static void ShowConsoleWindow()
        {
            EnsureConsoleAllocated();
            PInvoke.ShowWindow(consoleWindowHandle, SHOW_WINDOW_CMD.SW_SHOW);
            consoleShown = true;
        }

        public static void HideConsoleWindow()
        {
            EnsureConsoleAllocated();
            PInvoke.ShowWindow(consoleWindowHandle, SHOW_WINDOW_CMD.SW_HIDE);
            consoleShown = false;
        }

        internal static void EnsureConsoleAllocated()
        {
            if (!consoleWindowHandle.IsNull)
            {
                return;
            }
            if(!PInvoke.AllocConsole())
            {
                int err = Marshal.GetLastPInvokeError();
                throw new Win32Exception(err);
            }
            consoleWindowHandle = PInvoke.GetConsoleWindow();
            consoleShown = true;
        }

        public static bool IsWindowsKeyHeld()
        {
            return IsKeyHeld(Keys.LWin) || IsKeyHeld(Keys.RWin);
        }

        public static void SendInputs(params KeyboardInput[] inputs)
        {
            PInvoke.SendInput(inputs.SelectMany(input => input.ToInputs()).ToArray(), Marshal.SizeOf<INPUT>());
        }

        public static bool IsKeyHeld(Keys key)
        {
            return (PInvoke.GetAsyncKeyState((int)key) & (0b1 << 15)) > 0;
        }
    }
}
