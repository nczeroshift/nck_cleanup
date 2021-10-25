
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckWindow_win32.h"

#if defined(NCK_WINDOWS)

#include "../nckException.h"
#include "../nckUtils.h"
#include "resource.h"
#include <list>
#include <map>
#include <stringapiset.h>

#include <shobjidl.h> 

#if defined (_DEBUG) && defined(NCK_MEM_LEAKS)
    #include <crtdbg.h>
#endif

static Core::Mutex * p_WindowMutex = NULL;
static std::list<Core::Window_Win32*> *p_WindowManager = NULL;
static std::map<Core::KeyboardButton,int> *p_KeyboardButtons = NULL;
static float p_DisplayDensity = -1;

extern void initAppDataFolder();
extern void releaseAppDataFolder();

_CORE_BEGIN

void p_CreateButtonMap()
{
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_ESCAPE, VK_ESCAPE));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_ENTER, VK_RETURN));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_SPACE, VK_SPACE));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_UP, VK_UP));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_DOWN, VK_DOWN));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_LEFT, VK_LEFT));
    p_KeyboardButtons->insert(std::pair<KeyboardButton,int>(KEY_RIGHT, VK_RIGHT));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_A, 'A'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_B, 'B'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_C, 'C'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_D, 'D'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_E, 'E'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_F, 'F'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_G, 'G'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_H, 'H'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_I, 'I'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_J, 'J'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_K, 'K'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_L, 'L'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_M, 'M'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_N, 'N'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_O, 'O'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_P, 'P'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_Q, 'Q'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_R, 'R'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_S, 'S'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_T, 'T'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_U, 'U'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_V, 'V'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_W, 'W'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_X, 'X'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_Y, 'Y'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_Z, 'Z'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_0, '0'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_1, '1'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_2, '2'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_3, '3'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_4, '4'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_5, '5'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_6, '6'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_7, '7'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_8, '8'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_9, '9'));
    p_KeyboardButtons->insert(std::pair<KeyboardButton, int>(KEY_SHIFT, VK_SHIFT));
}

LRESULT WINAPI WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    Window_Win32 *active_wnd = NULL;

    p_WindowMutex->Lock();
    for(std::list<Window_Win32*>::iterator i = (*p_WindowManager).begin();i!=(*p_WindowManager).end();i++){
        if((*i)->m_Handle == hWnd)
            active_wnd = (*i);
    }
    p_WindowMutex->Unlock();

    if(active_wnd == NULL){
        Core::DebugLog("Window handle not found!\n");
        return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }

    switch ( uMsg )
    {
        case WM_KEYDOWN:
            MapSet(active_wnd->m_KeyStatus,int,ButtonStatus, UINT(wParam),BUTTON_STATUS_DOWN);
            break;

        case WM_KEYUP:
            MapSet(active_wnd->m_KeyStatus,int,ButtonStatus, UINT(wParam),BUTTON_STATUS_UP);
            break;

        case WM_LBUTTONDOWN:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_LEFT,BUTTON_STATUS_DOWN);    
            break;

        case WM_RBUTTONDOWN:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_RIGHT,BUTTON_STATUS_DOWN);        
            break;

        case WM_MBUTTONDOWN:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_MIDDLE,BUTTON_STATUS_DOWN);        
            break;

        case WM_LBUTTONUP:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_LEFT,BUTTON_STATUS_UP);        
            break;

        case WM_RBUTTONUP:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_RIGHT,BUTTON_STATUS_UP);        
            break;

        case WM_MBUTTONUP:
            MapSet(active_wnd->m_MouseBtnStatus,MouseButton,ButtonStatus,MOUSE_BUTTON_MIDDLE,BUTTON_STATUS_UP);
            break;

        case WM_MOUSEWHEEL:
        {
            short val = HIWORD(wParam);
            val /= WHEEL_DELTA;
            active_wnd->m_MouseWheel = val;
            break;
        }

        case WM_QUIT:
            PostQuitMessage(0);
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
        {
            if(SIZE_MINIMIZED==wParam)
                active_wnd->m_Status = WINDOW_IDLE;
            else if(SIZE_RESTORED==wParam || SIZE_MAXIMIZED==wParam)
                active_wnd->m_Status = WINDOW_ACTIVE;

            RECT tmpRect;
            GetClientRect(active_wnd->m_Handle,&tmpRect);
            active_wnd->m_Width = tmpRect.right;
            active_wnd->m_Height = tmpRect.bottom;
            break;
        }
            

        case WM_ACTIVATE:
            if(WA_INACTIVE == LOWORD(wParam))
                active_wnd->m_Status = WINDOW_IDLE;
            else
                active_wnd->m_Status = WINDOW_ACTIVE;
            break;

        case WM_ENTERSIZEMOVE:
            active_wnd->m_Status = WINDOW_IDLE;
            break;

        case WM_EXITSIZEMOVE:
            active_wnd->m_Status = WINDOW_ACTIVE;
            break;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

Window_Win32::Window_Win32(){
    m_Status = WINDOW_ACTIVE;
    m_Mutex = Core::CreateMutex();
    m_MouseWheel = 0;
}

Window_Win32::~Window_Win32(){
    DestroyCursor(m_Class.hCursor );
    DestroyIcon( m_Class.hIcon );
    DestroyIcon( m_Class.hIconSm );

    if(m_Handle )
    {
        DestroyWindow( m_Handle );
        m_Handle = (HWND)NULL;
    }

    if(!UnregisterClass("Application",m_Instance)){
        DWORD error_id = GetLastError();
    }
    
    p_WindowMutex->Lock();
    (*p_WindowManager).remove(this);
    p_WindowMutex->Unlock();

    SafeDelete(m_Mutex);
}

unsigned int Window_Win32::GetWidth(){
    return m_Width;
}

unsigned int Window_Win32::GetHeight(){
    return m_Height;
}

void Window_Win32::SetWidth(unsigned int iWidth){
    m_Width = iWidth;
}

void Window_Win32::SetHeight(unsigned int iHeight){
    m_Height = iHeight;
}

void Window_Win32::SetTitle(const std::string & title ){
    m_Title = title;
}

std::string Window_Win32::GetTitle(){
    return m_Title;
}

short Window_Win32::GetMouseWheel() {
    short ret = m_MouseWheel;
    m_MouseWheel = 0;
    return ret;
}

bool Window_Win32::Peek(bool peekAndWait){
    m_Mutex->Lock();

    if(m_TmpWidth!=m_Width || m_TmpHeight != m_Height){
        m_TmpWidth = m_Width;
        m_TmpHeight = m_Height;
    }

    if(m_Title != m_TmpTitle){
        SetWindowText(m_Handle,m_Title.c_str());
        m_TmpTitle = m_Title;
    }

    m_Mutex->Unlock();
    
    if(peekAndWait){
        MsgWaitForMultipleObjects(0, NULL, TRUE, 1000, QS_ALLEVENTS);
    }
            
    if( PeekMessage( &m_Message, NULL, 0U, 0U, PM_REMOVE ) )
    {
        if (m_Message.message==WM_QUIT)                    
        {
            return false;
        }
        else
        {
            TranslateMessage(&m_Message);                
            DispatchMessage(&m_Message);
            return true;
        }
    }
    else
        return true;
}

Point Window_Win32::GetMousePosition(){
    POINT gpoint = {0,0};
    
    GetCursorPos(&gpoint);
    ScreenToClient(m_Handle,&gpoint);

    Point rpoint = Point(gpoint.x,gpoint.y);
    return rpoint;
}

WindowStatus Window_Win32::GetStatus(){
    return m_Status;
}

ButtonStatus Window_Win32::GetMouseButtonStatus(MouseButton code){
    std::map<MouseButton,ButtonStatus>::iterator entry = m_MouseBtnStatus.find(code);
    if(entry == m_MouseBtnStatus.end())
        return BUTTON_STATUS_UP;
    return entry->second;
}

ButtonStatus Window_Win32::GetKeyStatus(KeyboardButton keycode){
    std::map<KeyboardButton,int>::iterator entry = (*p_KeyboardButtons).find(keycode);
    
    if(entry == (*p_KeyboardButtons).end())
        return BUTTON_STATUS_UP;

    std::map<int,Core::ButtonStatus>::iterator entry2 = m_KeyStatus.find(entry->second);

    if(entry2 == m_KeyStatus.end())
        return BUTTON_STATUS_UP;

    return entry2->second;
}

ButtonStatus Window_Win32::GetKeyCodeStatus(int code){
    if(m_KeyStatus.find(code)==m_KeyStatus.end())
        return BUTTON_STATUS_UP;
    return m_KeyStatus.find(code)->second;
}

void Window_Win32::SetCursorVisiblity(bool visiblity){
    ShowCursor(visiblity);
}

Window_Win32 *CreateWindow_Win32(const std::string & Title, unsigned int Width, unsigned int Height, unsigned int flags)
{
    Window_Win32 * m_Window = new Window_Win32();

    m_Window->m_Instance = GetModuleHandle( NULL );
    
    m_Window->m_Class.cbSize        = sizeof(WNDCLASSEX);
    m_Window->m_Class.style         = CS_HREDRAW | CS_VREDRAW;
    m_Window->m_Class.lpfnWndProc   = WndProc;
    m_Window->m_Class.cbClsExtra    = NULL;
    m_Window->m_Class.cbWndExtra    = NULL;
    m_Window->m_Class.hInstance     = m_Window->m_Instance;
    m_Window->m_Class.hIcon         = NULL;
    m_Window->m_Class.hIconSm       = NULL;
    m_Window->m_Class.hIcon         = (HICON)LoadImage( m_Window->m_Instance,
                    (LPCSTR)IDI_NCK_ENGINE_ICON, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE );
    m_Window->m_Class.hIconSm       = (HICON)LoadImage(m_Window->m_Instance, 
                    (LPCSTR)IDI_NCK_ENGINE_ICON, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE );
    m_Window->m_Class.hCursor       = LoadCursor( NULL, IDC_ARROW );
    m_Window->m_Class.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    m_Window->m_Class.lpszMenuName  = NULL;
    m_Window->m_Class.lpszClassName = "Application";

    m_Window->m_Width = Width;
    m_Window->m_Height = Height;

    if(!RegisterClassEx( &m_Window->m_Class ))
    {
        DWORD error_id = GetLastError();
        Core::DebugLog("Warning at CreateWindow_Win32(), window class already created.\n");
    }

    RECT rc = { 0, 0, (long)Width, (long)Height };

    bool isFullscreen = (flags & WINDOW_FULLSCREEN) != 0;
    bool isMaximized = (flags & WINDOW_MAXIMIZED) != 0;
    bool hasMaximize = (flags & WINDOW_MAXIMIZED_BUTTON) != 0;
    bool hasMinimize = (flags & WINDOW_MINIMIZED_BUTTON) != 0;
    bool hasClose = (flags & WINDOW_CLOSED_BUTTON) != 0;

    if (isFullscreen) {
        m_Window->m_Flags = WS_POPUP;

        DEVMODE dmSettings;
        memset(&dmSettings,0,sizeof(dmSettings));

        if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings)) {
            delete m_Window;
            THROW_EXCEPTION("Unable to enumerate display settings");
        }

        dmSettings.dmPelsWidth = Width;
        dmSettings.dmPelsHeight = Height;
        dmSettings.dmDisplayFrequency = 60;

        int result = ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);

        if (result != DISP_CHANGE_SUCCESSFUL) {
            delete m_Window;
            THROW_EXCEPTION("Unable to change display settings");
        }
    }
    else
    {
        m_Window->m_Flags = WS_CAPTION | WS_SYSMENU;

        if (isMaximized)
            m_Window->m_Flags |= WS_MAXIMIZE;

        if (hasMaximize)
            m_Window->m_Flags |= WS_MAXIMIZEBOX | WS_OVERLAPPED | WS_THICKFRAME;

        if(hasMinimize)
            m_Window->m_Flags |= WS_MINIMIZEBOX | WS_OVERLAPPED | WS_THICKFRAME;
    }

    AdjustWindowRectEx( &rc, m_Window->m_Flags, FALSE, WS_EX_APPWINDOW );

    m_Window->m_TmpTitle = m_Window->m_Title = Title;
    m_Window->m_TmpWidth = m_Window->m_Width;
    m_Window->m_TmpHeight = m_Window->m_Height;

    // Create the application's window
    m_Window->m_Handle = CreateWindowEx( WS_EX_APPWINDOW, "Application", Title.c_str(), m_Window->m_Flags,
        ( ( GetSystemMetrics(SM_CXSCREEN) - Width ) / 2 )-1, ( ( GetSystemMetrics(SM_CYSCREEN) - Height ) / 2)-1, 
        rc.right - rc.left, rc.bottom - rc.top, GetDesktopWindow(), NULL, m_Window->m_Instance, NULL );

    if (!m_Window->m_Handle)
    {
        delete m_Window;
        THROW_EXCEPTION("Unable to create window");
    }

    if (isFullscreen) {
        MONITORINFO monitor_info;
        monitor_info.cbSize = sizeof(monitor_info);
        GetMonitorInfo(MonitorFromWindow(m_Window->m_Handle, MONITOR_DEFAULTTONEAREST),
            &monitor_info);
        
        RECT rect = monitor_info.rcMonitor;
        SetWindowPos(m_Window->m_Handle, NULL, rect.left, rect.top,
            rect.right- rect.left, rect.bottom-rect.top,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }

    p_WindowMutex->Lock();
    (*p_WindowManager).push_back(m_Window);
    p_WindowMutex->Unlock();

    ShowWindow( m_Window->m_Handle, SW_SHOW );
    UpdateWindow( m_Window->m_Handle );

    return m_Window;
}

float Window::GetDisplayDensity() {
    const float DEFAULT_DPI = 96.0;
    if (p_DisplayDensity < 0.0){
        HDC screen = GetDC(0);
        FLOAT dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
        ReleaseDC(0, screen);
        p_DisplayDensity = dpiX / DEFAULT_DPI;
    }
    return p_DisplayDensity;
}

_CORE_END

void showOpenDialog() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    const wchar_t* wbuf = pszFilePath;
                    int len = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, 0, 0, 0, 0);

                    std::string buf(len, 0);

                    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, &buf[0], len, 0, 0);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //showOpenDialog();

    SetProcessDPIAware();

    initAppDataFolder();
    p_WindowMutex = Core::CreateMutex();

    p_WindowManager = new std::list<Core::Window_Win32*>;
    p_KeyboardButtons = new std::map<Core::KeyboardButton,int>;

    Core::p_CreateButtonMap();

    std::vector<std::string> * params = new std::vector<std::string>();
    Core::StringSplit(std::string(lpCmdLine)," ",params);
    Core::Application_Main_Win32(*params);
    
    SafeDelete(params);
    SafeDelete(p_KeyboardButtons);
    SafeDelete(p_WindowManager);

    releaseAppDataFolder();
    SafeDelete(p_WindowMutex);

    #if defined(_DEBUG) && defined(NCK_MEM_LEAKS)
        ::_CrtDumpMemoryLeaks();
    #endif

    return 1;
}

//#ifdef _MSC_VER == 1900
//FILE _iob[] = { *stdin, *stdout, *stderr };
//extern "C" FILE * __cdecl __iob_func(void){
//    return _iob;
//}
//#endif

#endif // #if defined(NCK_WINDOWS)
