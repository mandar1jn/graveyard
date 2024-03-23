using System.Runtime.InteropServices;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;

namespace WindowsMods.WinAPI.Triggers
{
    public class TaskbarTrigger
    {
        private static DateTime lastHandleUpdate = DateTime.MinValue;

        static readonly List<HWND> taskbars = new();

        public static bool IsMouseOverTaskbar(Point mousePos)
        {
            TryUpdateHandles();

            foreach(HWND taskbar in taskbars)
            {
                PInvoke.GetWindowRect(taskbar, out RECT lpRect);

                Rectangle rect = lpRect;

                Console.WriteLine(rect);
                Console.WriteLine(mousePos);

                if (rect.Contains(mousePos))
                {
                    return true;
                }
            }

            return false;
        }

        public static void TryUpdateHandles()
        {
            if((DateTime.Now - lastHandleUpdate).TotalSeconds > 15)
            {
                HWND desktopWindow = PInvoke.GetDesktopWindow();

                taskbars.Clear();
                PInvoke.EnumChildWindows(desktopWindow, new WNDENUMPROC(ChildEnumCallback), 0);

                lastHandleUpdate = DateTime.Now;
            }
        }

        public unsafe static BOOL ChildEnumCallback(HWND hwnd, LPARAM lParam)
        {
            IntPtr ptr = Marshal.StringToHGlobalAuto(new string('\0', 1024));
            PWSTR title = new((char*)ptr.ToPointer());
            int outLength = PInvoke.GetClassName(hwnd, title, 1024);

            if (outLength > 0)
            {
                if (title.ToString().Contains("Shell_TrayWnd") || title.ToString().Contains("Shell_SecondaryTrayWnd"))
                {
                    taskbars.Add(hwnd);
                }
            }

            Marshal.FreeHGlobal(ptr);

            return true;
        }
    }
}
