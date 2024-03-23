using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Win32;
using Windows.Win32.Media.Audio;
using Windows.Win32.Media.Audio.Endpoints;
using Windows.Win32.System.Com;
using Windows.Win32.System.Com.StructuredStorage;

namespace WindowsMods.WinAPI
{
    public static class AudioController
    {
        public static int GetVolume()
        {
            IMMDeviceEnumerator deviceEnumerator = (IMMDeviceEnumerator)Activator.CreateInstance(Type.GetTypeFromCLSID(typeof(MMDeviceEnumerator).GUID)!)!;

            deviceEnumerator.GetDefaultAudioEndpoint(EDataFlow.eRender, ERole.eMultimedia, out IMMDevice immDevice);

            immDevice.Activate(typeof(IAudioEndpointVolume).GUID, CLSCTX.CLSCTX_ALL, new PROPVARIANT(), out object audioInterfaceObject);

            IAudioEndpointVolume volumeEndpoint = (IAudioEndpointVolume)audioInterfaceObject;

            volumeEndpoint.GetMasterVolumeLevelScalar(out float volume);
            Console.WriteLine(volume);

            return 0;
        }

    }
}
