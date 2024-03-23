using System.Reflection;

namespace WindowsMods.Core
{
    public class ModLoader
    {
        private ModLoader()
        {

        }

        private static ModLoader? instance;
        public static ModLoader Instance
        {
            get
            {
                instance ??= new ModLoader();
                return instance;
            }
        }

        readonly List<Mod> mods = new();

        public void LoadMods()
        {
            string modDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "mods");

            string[] files = Directory.EnumerateFiles(modDir).Where(file => file.EndsWith(".dll")).ToArray();

            List<Assembly> assemblies = new();

            foreach (string file in files)
            {
                assemblies.Add(Assembly.LoadFrom(file));
            }

            foreach (Assembly assembly in assemblies)
            {
                Type[] modTypes = assembly.GetTypes().Where(type => !type.IsAbstract && type.IsClass && type.IsSubclassOf(typeof(Mod))).ToArray();

                foreach (Type modType in modTypes)
                {
                    if (modType.GetConstructor(Type.EmptyTypes)?.Invoke(Array.Empty<object>()) is Mod mod)
                    {
                        mods.Add(mod);
                    }
                }
            }
        }

        public Mod[] GetModList()
        {
            return mods.ToArray();
        }

    }
}
