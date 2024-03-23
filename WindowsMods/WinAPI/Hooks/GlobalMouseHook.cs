using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;
using WindowsMods.WinAPI.Extensions;

namespace WindowsMods.WinAPI.Hooks
{
    public class GlobalMouseHook : IDisposable
    {
        public GlobalMouseHook(MouseHookCallback hookCallback)
        {
            this.hookCallback = hookCallback;
            hook = new GlobalHook(WINDOWS_HOOK_ID.WH_MOUSE_LL, new GlobalHook.GlobalHookCallback(Callback));
        }

        public void Callback(int nCode, WPARAM wParam, LPARAM lParam, out bool suppress)
        {
            hookCallback.Invoke(wParam, lParam.ToMouseInfo(), out bool shouldSuppress);

            suppress = shouldSuppress;
        }

        public void Dispose()
        {
            hook.Dispose();
        }

        private readonly GlobalHook hook;

        private readonly MouseHookCallback hookCallback;

        public delegate void MouseHookCallback(WPARAM wParam, MSLLHOOKSTRUCT mouseData, out bool suppress);
    }
}
