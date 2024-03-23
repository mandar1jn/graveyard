using System.Diagnostics;
using WindowsMods.Core;
using WindowsMods.Properties;

namespace WindowsMods.UI
{
    internal class TrayIcon : ApplicationContext
    {
        public TrayIcon()
        {
            notifyIcon = new NotifyIcon
            {
                Text = "Windows Mods",
                Icon = Resources.tray_icon,
                ContextMenuStrip = new ContextMenuStrip(),
                Visible = true
            };

            ToolStripMenuItem websiteItem = new("Website") {Text = "View my website"};
            websiteItem.Click += OpenWebsite;
            notifyIcon.ContextMenuStrip.Items.Add(websiteItem);

            ToolStripMenuItem modListItem = new("ModList") { Text = "List installed mods" };
            modListItem.Click += ListMods;
            notifyIcon.ContextMenuStrip.Items.Add(modListItem);

            ToolStripMenuItem exitItem = new("Exit") { Text = "Exit" };
            exitItem.Click += Exit;
            notifyIcon.ContextMenuStrip.Items.Add(exitItem);
        }

        private static void OpenWebsite(object? sender, EventArgs e)
        {
            Process.Start(new ProcessStartInfo("https://marijnkneppers.dev") { UseShellExecute = true });
        }

        private static void ListMods(object? sender, EventArgs e)
        {
            string mods = "";
            foreach(Mod mod in ModLoader.Instance.GetModList())
            {
                mods += $"{mod.Name} - {mod.Version.major}.{mod.Version.minor}.{mod.Version.patch}\n";
            }

            MessageBox.Show(mods, "Installed mods");
        }

        private static void Exit(object? sender, EventArgs e)
        {
            Application.Exit();
        }

        private readonly NotifyIcon notifyIcon;
    }
}
