
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#include "nckDemo_Selector.h"
 
//#define RUN_DEMO 37

class GraphicRendering : public virtual Core::Threadable, public virtual DemoSelector_Callback
{
public:
    GraphicRendering(Core::Window * window) : Threadable(){
        wnd = window;
        dev = NULL;
        mutex = Core::CreateMutex();
        
        demoSelector = NULL;
        demoActive = NULL;
        demoDestroy = NULL;
        demoLoad = NULL;
        pressedEscape = false;
    }
    
    ~GraphicRendering(){
        SafeDelete(mutex);
    }

    void SelectDemo(int demoId)
    {
        Demo * tmp = NULL;
        tmp = DemoCreate(demoId, wnd, dev);
        demoLoad = tmp;
    }

    void Run(){
        wnd->SetTitle("nctoolkit");
        
        try{
            dev = Graph::CreateDevice(wnd,Graph::DEVICE_AUTO,wnd->GetWidth(),wnd->GetHeight());
            dev->ClearFlags(Graph::BUFFER_COLOR_BIT|Graph::BUFFER_DEPTH_BIT);
            dev->ClearColor(0.5, 0.5, 0.5);
        }
        catch(const Core::Exception & ex){
            ex.PrintStackTrace();
            SafeDelete(dev);
            return;
        }

        Demo * tmp = NULL;

        mutex->Lock();

        try {
#ifdef RUN_DEMO
            tmp = DemoCreate(RUN_DEMO,wnd, dev);
            tmp->Load();
            demoActive = tmp;
#else
            tmp = CreateDemo_Selector(wnd, dev);
            tmp->Load();
            ((DemoSelector*)tmp)->SetCallback(this);
            demoSelector = tmp;
#endif
        }
        catch(const Core::Exception & ex){
            ex.PrintStackTrace();
            SafeDelete(tmp);
            SafeDelete(dev);
        }
		

        mutex->Unlock();
                

        while(!IsTearingDown())
        {
			dev->Clear();
			dev->PresentAll();
            mutex->Lock();
            
            // Dealocate memory for previous demo.
            if(demoDestroy){
                SafeDelete(demoDestroy);
                wnd->SetTitle("nctoolkit");
            }

            // Load next demo in the graphics thread.
            if(demoLoad){
                try{
                    demoLoad->Load();
                    demoActive = demoLoad;
                }catch(const Core::Exception & ex){
                    ex.PrintStackTrace();
                    SafeDelete(demoLoad);
                    wnd->SetTitle("nctoolkit");
                }

                demoLoad = NULL;
            }

            if(demoActive) 
                demoActive->Render();
            else if(demoSelector)
                demoSelector->Render();
				
            mutex->Unlock();
        }

        mutex->Lock();
        SafeDelete(demoActive);
        SafeDelete(demoSelector);
        mutex->Unlock();

        //SafeDelete(dev);
    }

    void UpdateWndInput()
    {
		if (wnd->GetKeyStatus(Core::KEY_ESCAPE) != Core::BUTTON_STATUS_UP)
			Teardown();
		
        mutex->Lock();

        if(demoActive)
            demoActive->UpdateWndEvents();
        else if(demoSelector)
            demoSelector->UpdateWndEvents();

        if(wnd->GetKeyStatus(Core::KEY_ESCAPE) != Core::BUTTON_STATUS_UP){
            if(!pressedEscape){
                if(demoActive){
                    demoDestroy = demoActive;
                    demoActive = NULL;
    #ifdef RUN_DEMO
                    Teardown();
    #endif
                }
                else if(demoSelector){
                    Teardown();
                }
            }
            pressedEscape = true;
        }
        else{
            pressedEscape = false;
        }

        mutex->Unlock();
    }

private:

    bool            pressedEscape;
    Demo            * demoSelector, * demoActive, * demoDestroy, * demoLoad;
    Core::Mutex     *mutex;
    Graph::Device   *dev;
    Core::Window    *wnd;
};

void Core::Application_Main(const std::vector<std::string> & CmdLine)
{
    //Core::FileReader * data = Core::FileReader::Open("data.tar");
    //Core::TarReader * tar = new Core::TarReader(data);
    //tar->Mount();

    // Create a window.
    Core::Window * wnd = Core::CreateWindow("nctoolkit", 1920, 1080, Core::WINDOW_MINIMIZED_BUTTON | Core::WINDOW_MAXIMIZED_BUTTON | Core::WINDOW_MAXIMIZED);

    // Create the thread where the graphics rendering will run.
    GraphicRendering * grThread = new GraphicRendering(wnd);

    // Start graphics thread.
    grThread->Start();

    // Get window events.
    while(wnd->Peek(true))
    {
        // If on selector frontend, escape will tell
        // the thread to enter shutdown.
        if(!grThread->IsRunning())
            break;

        grThread->UpdateWndInput();
    }

    // Tell the thread to end and wait.
    grThread->TearDownAndJoin();

    // Release stuff from memory.
    SafeDelete(grThread);
    SafeDelete(wnd);
}

