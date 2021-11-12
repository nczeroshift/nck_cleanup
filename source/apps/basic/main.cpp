
#include "nckWindow.h"
#include "nckGraphics.h"
#include "nckException.h"
#include "nckUtils.h"
#include "nckThread.h"

class App : public virtual Core::Threadable
{
private:
    Graph::Device * dev;
    Core::Window * wnd;

public:
    App(Core::Window* window){
        this->wnd = window;
    }

    virtual ~App() {

    }


    void Run() {
        dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());

        dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);
        dev->ClearColor(0.5, 0.5, 0.5);

        while (!IsTearingDown())
        {
            dev->Clear();
            dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

           
            dev->PresentAll();
        }

        SafeDelete(dev);
        

    }

    void UpdateInputs() {
        if(wnd->GetKeyStatus(Core::KeyboardButton::KEY_ESCAPE)!=Core::ButtonStatus::BUTTON_STATUS_UP)
            Teardown();
    }


};


void Core::Application_Main(const std::vector<std::string> & CmdLine)
{
	Core::Window * wnd = Core::CreateWindow("App",
        1024, 800, 
        Core::WINDOW_RESTORE_BUTTON | Core::WINDOW_MINIMIZED_BUTTON | Core::WINDOW_MAXIMIZED_BUTTON);

    App * app = new App(wnd);
	app->Start();

	while (wnd->Peek(true))
	{
		if (!app->IsRunning())
			break;

		app->UpdateInputs();
	}

	app->TearDownAndJoin();

	SafeDelete(app);
	SafeDelete(wnd);
}


