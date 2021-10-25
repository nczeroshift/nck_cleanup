#include "nckWindow.h"
#include "nckGraphics.h"
#include "nckThread.h"
#include "nckUtils.h"
#include "nckException.h"
#include "nckGraphicsUtils.h"
#include "nckWidgetRenderer.h"

class GraphicRendering : public virtual Core::Threadable
{
public:
    GraphicRendering(Core::Window* window) : Threadable() {
        wnd = window;
        dev = NULL;
        mutex = Core::CreateMutex();
        font = NULL;
    }

    ~GraphicRendering() {
        SafeDelete(mutex);
    }

    Gui::Font * font;

    void Run() {
        wnd->SetTitle("nctoolkit");

        try {
            dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());
            dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);
            dev->ClearColor(0.5, 0.5, 0.5);
            
            font = new Gui::Font(dev);
            font->Load("sans_serif_10");
        }
        catch (const Core::Exception& ex) {
            ex.PrintStackTrace();
            SafeDelete(dev);
            return;
        }


        while (!IsTearingDown())
        {
            dev->Clear();

            dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

            dev->MatrixMode(Graph::MATRIX_PROJECTION);
            dev->Identity();
            dev->Ortho2D(wnd->GetWidth(), wnd->GetHeight());

            dev->MatrixMode(Graph::MATRIX_VIEW);
            dev->Identity();

            //dev->Rotate
            //dev->Translate(-7.35889, 6.92579, -4.95831);

            dev->MatrixMode(Graph::MATRIX_MODEL);
            dev->Identity();

            dev->Enable(Graph::STATE_BLEND);
            dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
            dev->Disable(Graph::STATE_DEPTH_TEST);

            dev->Color(255, 255, 255);
            Graph::Render_Square(dev, 0, 0, 128, 128);

            dev->Color(0,0,0);
            font->Draw(10,10,"sup");

            dev->PresentAll();
            mutex->Lock();

        }


        mutex->Unlock();

        SafeDelete(dev);
    }

    void UpdateWndInput()
    {
        if (wnd->GetKeyStatus(Core::KEY_ESCAPE) != Core::BUTTON_STATUS_UP)
            Teardown();

    }

private:
    
    Core::Mutex* mutex;
    Graph::Device* dev;
    Core::Window* wnd;
};

void Core::Application_Main(const std::vector<std::string> & CmdLine)
{
    Core::Window* wnd = Core::CreateWindow("nctoolkit", 1024, 768,
    Core::WINDOW_MINIMIZED_BUTTON | Core::WINDOW_MAXIMIZED_BUTTON);

    GraphicRendering* grThread = new GraphicRendering(wnd);

    grThread->Start();

    while (wnd->Peek(true))
    {
        if (!grThread->IsRunning())
            break;

        grThread->UpdateWndInput();
    }

    grThread->TearDownAndJoin();

    SafeDelete(grThread);
    SafeDelete(wnd);
}
