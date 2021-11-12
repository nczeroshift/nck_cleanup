
#include "App.h"
#include "Utils.h"
#include "nckStringUtils.h"

void Core::Application_Main(const std::vector<std::string> & CmdLine)
{
	Core::Window * wnd = Core::CreateWindow("Bitmap Font Tool", 1024, 800, 
    Core::WINDOW_RESTORE_BUTTON | Core::WINDOW_MINIMIZED_BUTTON | Core::WINDOW_MAXIMIZED_BUTTON);

	App * app = new App(wnd);

	int pFistLine = -1;
	int pLineHeight = -1;
	int pOutputWidth = -1;
	int pOutputHeight = -1;
	std::string pOutputImageFile;
	std::string pOutputPathFile;

	for (int i = 0; i < CmdLine.size(); i += 2) {
		if (i + 1 >= CmdLine.size())
			break;
		
		std::string key = CmdLine[i];
		std::string value = CmdLine[i + 1];

		if (key == "-i") app->SetInputFile(value);
		if (key == "-o") pOutputImageFile = value;
		if (key == "-op") pOutputPathFile = value;
		if (key == "-dm") app->SetDetectMargin(Core::StringToInt(value));
		if (key == "-om") app->SetOutputMargin(Core::StringToInt(value));
		if (key == "-ms") app->SetMonospace("true" == value);
		if (key == "-sw") app->SetSpaceWidth(Core::StringToInt(value));
		if (key == "-dfl") pFistLine = Core::StringToInt(value);
		if (key == "-dlh") pLineHeight = Core::StringToInt(value);
		if (key == "-ow") pOutputWidth = Core::StringToInt(value);
		if (key == "-oh") pOutputHeight = Core::StringToInt(value);
		if (key == "-ch") app->SetCharactersHeight(Core::StringToInt(value));
		if (key == "-em") app->SetExtraMargin(Core::StringToInt(value));
	}

	if (!pOutputImageFile.empty() && !pOutputPathFile.empty())
		app->SetOutputFiles(pOutputImageFile, pOutputPathFile);

	if (pFistLine != -1 && pLineHeight != -1)
		app->SetFixedLine(pFistLine,pLineHeight);
	
	if (pOutputWidth != -1 && pOutputHeight != -1)
		app->SetOutputSize(pOutputWidth, pOutputHeight);

	app->Start();

	while (wnd->Peek(true))
	{
		if (!app->IsRunning())
			break;

		app->UpdateWndInput();
	}

	app->TearDownAndJoin();

	SafeDelete(app);
	SafeDelete(wnd);
}


