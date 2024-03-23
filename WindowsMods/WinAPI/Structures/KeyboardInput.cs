using Windows.Win32.UI.Input.KeyboardAndMouse;

namespace WindowsMods.WinAPI.Structures
{
    public struct KeyboardInput
    {

        public VIRTUAL_KEY key;
        
        public KEYBD_EVENT_FLAGS? action;

        internal List<INPUT> ToInputs()
        {
            List<INPUT> list = new();

            if(action == null)
            {
                INPUT input = new()
                {
                    type = INPUT_TYPE.INPUT_KEYBOARD,
                    Anonymous =
                    {
                        ki =
                        {
                            wVk = key,
                        }
                    }
                };

                list.Add(input);

                input = new()
                {
                    type = INPUT_TYPE.INPUT_KEYBOARD,
                    Anonymous =
                    {
                        ki =
                        {
                            dwFlags = KEYBD_EVENT_FLAGS.KEYEVENTF_KEYUP,
                            wVk = key
                        }
                    }
                };

                list.Add(input);
            }
            else
            {
                INPUT input = new()
                {
                    type = INPUT_TYPE.INPUT_KEYBOARD,
                    Anonymous =
                    {
                        ki =
                        {
                            dwFlags = (KEYBD_EVENT_FLAGS)action,
                            wVk = key
                        }
                    }
                };

                list.Add(input);
            }

            return list;
        }

    }
}
