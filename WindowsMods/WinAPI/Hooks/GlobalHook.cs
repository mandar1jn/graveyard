using System.Diagnostics;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;

namespace WindowsMods.WinAPI.Hooks
{
    public class GlobalHook : IDisposable
    {
        public GlobalHook(WINDOWS_HOOK_ID hookId, GlobalHookCallback callback)
        {
            callbackDelegate = callback;
            hookProc = new HOOKPROC(Callback);
            libraryHandle = PInvoke.GetModuleHandle((string)null);
            handle = PInvoke.SetWindowsHookEx(hookId, hookProc, PInvoke.GetModuleHandle(Process.GetCurrentProcess().MainModule!.ModuleName), 0);
        }

        private LRESULT Callback(int nCode, WPARAM wParam, LPARAM lParam)
        {
            if(nCode == 0)
            {
                callbackDelegate(nCode, wParam, lParam, out bool suppress);
                if(suppress)
                {
                    return new LRESULT(1);
                }
            }
            return PInvoke.CallNextHookEx(handle, nCode, wParam, lParam);
        }

        public void Dispose()
        {
            handle.Dispose();
            libraryHandle.Dispose();
        }

        private readonly GlobalHookCallback callbackDelegate;

        private readonly HOOKPROC hookProc;

        private readonly UnhookWindowsHookExSafeHandle handle;

        private readonly FreeLibrarySafeHandle libraryHandle;

        public delegate void GlobalHookCallback(int nCode, WPARAM wParam, LPARAM lParam, out bool suppress);
    }
}
