
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckGraphics.h"

#if defined NCK_OPENGLX2
    #include "glx2/nckGraphics_glx2.h"
#endif

double m_pGlobalTime = 0;

_GRAPHICS_BEGIN

void Program::SetGlobalTime(double time) {
    m_pGlobalTime = time;
}

double Program::GetGlobalTime() {
    return m_pGlobalTime;
}


Device* CreateDevice(Core::Window *wnd,DeviceType type,
                                unsigned int width, unsigned int height,
                                unsigned int antialiasing,
                                bool fullscreen)
{
    return (Device*)CreateDevice_GLX2(wnd,width,height,antialiasing,fullscreen);
    return NULL;
}

_GRAPHICS_END
