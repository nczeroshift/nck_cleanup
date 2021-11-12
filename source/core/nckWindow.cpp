
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckCoreConfig.h"

#if defined(NCK_WINDOWS)
#include "win32/nckwindow_win32.h"
#undef CreateWindow
#elif defined(NCK_LINUX)
#include "linux/nckWindow_linux.h"
#include <stdio.h>
#include <stdlib.h>
#elif defined(NCK_MACOSX)
#include "macosx/nckWindow_macosx.h"
#endif

_CORE_BEGIN


Window *CreateWindow(const std::string & title, unsigned int width, unsigned int height, uint32_t flags)
{
#if defined(NCK_WINDOWS)
    return (Window*)CreateWindow_Win32(title, width, height, flags);
#elif defined(NCK_LINUX)
    return (Window*)CreateWindow_Linux(title, width, height, fullscreen);
#elif defined(NCK_MACOSX)
    return CreateWindow_MacOSX(title, width, height, fullscreen);
#endif
}

#if defined(NCK_WINDOWS)
void Application_Main_Win32(const std::vector<std::string> & cmdLine){
    Application_Main(cmdLine);
}
#elif defined(NCK_LINUX)
void Application_Main_Linux(const std::vector<std::string> & cmdLine){
    Application_Main(cmdLine);
}
#elif defined(NCK_MACOSX)
void Application_Main_MacOSX(const std::vector<std::string> & cmdLine){
    Application_Main(cmdLine);
}
#endif    

void MsgBox(const std::string & text,const std::string & title){
#if defined(NCK_WINDOWS)
    MessageBox(NULL,text.c_str(),title.c_str(), MB_OK);
#elif defined(NCK_LINUX)
    printf("MSGBOX %s %s\n",title.c_str(),text.c_str());
#elif defined(NCK_MACOSX)
    MessageBox_MacOSX(text,title);
#endif
}

_CORE_END

