#include "ExtractorApplication.h"
#include "ui/MenuBar.h"
#include "ui/FileBrowser.h"
#include "ui/Aboutwindow.h"
#include "PortableFileDialogs.h"
#include "RMDPArchive.h"

using namespace ExtractorCore;

static ExtractorApplication application = ExtractorApplication("AWExtractor");

static void OpenFileCallback()
{
	auto selection = pfd::open_file("Open Northlight Archive", ".", { "Northlight Archive", "*.rmdp",
								  "All Files", "*" }, pfd::opt::multiselect).result();

	for (auto& file : selection)
	{
		std::shared_ptr<AWExtractor::RMDPArchive> arch = std::make_shared<AWExtractor::RMDPArchive>(file);
		ExtractorCore::UI::FileBrowser::archives.push_back(arch);
	}
}

static void RegisterMenuBar()
{
	UI::MenuBarMenu fileMenu = UI::MenuBarMenu("File");

	UI::MenuBarItem openFileItem = UI::MenuBarItem("Open", OpenFileCallback);

	fileMenu.AddItem(openFileItem);

	UI::MenuBar::AddMenu(fileMenu);
}

static void SetAboutInfo()
{
	ExtractorCore::UI::AboutInfo info = {};

	info.toolName = "AWExtractor";

	ExtractorCore::UI::AboutWindow::info = info;
}

int main(char argc, char** argv)
{
	RegisterMenuBar();
	SetAboutInfo();

	application.Run(argc, argv);

	return 0;
}