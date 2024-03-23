using System.Runtime.InteropServices;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.Input.KeyboardAndMouse;
using Windows.Win32.UI.WindowsAndMessaging;
using WindowsMods.WinAPI.Structures;

namespace WindowsMods.WinAPI.Hooks
{
    public class GlobalKeyboardSuppressionHook : IDisposable
    {
        private GlobalHook? hook;
        private bool incomingSuppression = false;
        private readonly VIRTUAL_KEY key;

        public GlobalKeyboardSuppressionHook(VIRTUAL_KEY key)
        {
            this.key = key;
        }

        public void SuppressNext()
        {
            if(hook != null)
            {
                return;
            }
            hook = new GlobalHook(WINDOWS_HOOK_ID.WH_KEYBOARD_LL, Callback);
        }

        public void Disable()
        {
            if(hook != null)
            {
                hook.Dispose();
                hook = null;
                incomingSuppression = false;
            }
        }

        private void Callback(int nCode, WPARAM wParam, LPARAM lParam, out bool suppress)
        {
            KBDLLHOOKSTRUCT keyData = Marshal.PtrToStructure<KBDLLHOOKSTRUCT>(lParam);

            suppress = false;

            if (wParam == PInvoke.WM_KEYUP && keyData.vkCode == (uint)key && !incomingSuppression)
            {
                incomingSuppression = true;

                WinAPIController.SendInputs(new KeyboardInput[]
                {
                new(){key = key, action = 0},
                new(){key = VIRTUAL_KEY.VK_INSERT},
                new(){key = key, action = KEYBD_EVENT_FLAGS.KEYEVENTF_KEYUP},
                });
                suppress = true;
            }
            else if (wParam == PInvoke.WM_KEYUP && keyData.vkCode == (uint)VIRTUAL_KEY.VK_INSERT && incomingSuppression)
            {
                Disable();
                suppress = true;
            }
        }

        public void Dispose()
        {
            hook?.Dispose();
        }
    }
}
