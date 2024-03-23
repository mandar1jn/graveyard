using WindowsMods.Core;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;
using WindowsMods.WinAPI.Hooks;
using WindowsMods.WinAPI.Extensions;
using Windows.Win32;
using Windows.Win32.UI.Input.KeyboardAndMouse;
using WindowsMods.WinAPI;
using WindowsMods.WinAPI.Structures;
using WindowsMods.WinAPI.Triggers;

namespace VolumeScroll
{
    public class VolumeScroll : Mod
    {
        public override string Name => "Volume Scroll";

        public override WindowsMods.Core.Version Version => new() { major = 0, minor = 0, patch = 0 };

        private readonly GlobalMouseHook mouseHook;

        private readonly GlobalKeyboardSuppressionHook leftSuppressionHook;
        private readonly GlobalKeyboardSuppressionHook rightSuppressionHook;

        public VolumeScroll()
        {
            Console.WriteLine("Initialized Volume Scroll!!");
            mouseHook = new GlobalMouseHook(MouseCallback);
            leftSuppressionHook = new GlobalKeyboardSuppressionHook(VIRTUAL_KEY.VK_LWIN);
            rightSuppressionHook = new GlobalKeyboardSuppressionHook(VIRTUAL_KEY.VK_RWIN);
        }

        private void MouseCallback(WPARAM wParam, MSLLHOOKSTRUCT mouseData, out bool suppress)
        {
            AudioController.GetVolume();
            if(wParam.Value == PInvoke.WM_MOUSEWHEEL)
            {
                if (WinAPIController.IsWindowsKeyHeld())
                {
                    bool movedUp = mouseData.WasWheelMovedUp();
                    bool movedDown = mouseData.WasWheelMovedDown();
                    if (movedUp || movedDown)
                    {
                        if (movedUp)
                        {
                            IncreaseVolume(2);
                        }
                        else if (movedDown)
                        {
                            DecreaseVolume(2);
                        }

                        rightSuppressionHook?.SuppressNext();
                        leftSuppressionHook?.SuppressNext();
                    }
                }
                else
                {
                    PInvoke.GetCursorPos(out Point mousePos);

                    if(TaskbarTrigger.IsMouseOverTaskbar(mousePos))
                    {
                        bool movedUp = mouseData.WasWheelMovedUp();
                        bool movedDown = mouseData.WasWheelMovedDown();
                        if (movedUp || movedDown)
                        {
                            if (movedUp)
                            {
                                IncreaseVolume(2);
                            }
                            else if (movedDown)
                            {
                                DecreaseVolume(2);
                            }
                        }
                    }
                }
            }

            suppress = false;
        }

        private static void IncreaseVolume(int keyPresses)
        {
            KeyboardInput[] inputs = Enumerable.Repeat(new KeyboardInput { key = VIRTUAL_KEY.VK_VOLUME_UP }, keyPresses).ToArray();

            WinAPIController.SendInputs(inputs);
        }

        private static void DecreaseVolume(int keyPresses)
        {
            KeyboardInput[] inputs = Enumerable.Repeat(new KeyboardInput { key = VIRTUAL_KEY.VK_VOLUME_DOWN }, keyPresses).ToArray();

            WinAPIController.SendInputs(inputs);
        }
    }
}
