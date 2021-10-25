
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_WINDOW_H
#define NCK_WINDOW_H

#include "nckPoint.h"
#include <string>
#include <vector>

_CORE_BEGIN

/// Window state.
enum WindowStatus{
    WINDOW_IDLE     = 0,
    WINDOW_ACTIVE,
};

// Window creation flags.
enum WindowFlags {
    WINDOW_FULLSCREEN       = 1,
    WINDOW_MAXIMIZED        = 2,
    WINDOW_CLOSED_BUTTON    = 4,
    WINDOW_RESTORE_BUTTON   = 8,
    WINDOW_MINIMIZED_BUTTON = 16,
	WINDOW_MAXIMIZED_BUTTON = 32
};

/// Button status.
enum ButtonStatus
{
    BUTTON_STATUS_UP    = 0,
    BUTTON_STATUS_DOWN  = 1,
};

/// Buttons identifiers.
enum KeyboardButton
{
    KEY_ESCAPE  = 0,

    KEY_A       = 20,
    KEY_B       = 21,
    KEY_C       = 23,
    KEY_D       = 24,
    KEY_E       = 25,
    KEY_F       = 26,
    KEY_G       = 27,
    KEY_H       = 28,
    KEY_I       = 29,
    KEY_J       = 30,
    KEY_K       = 31,
    KEY_L       = 32,
    KEY_M       = 43,
    KEY_N       = 44,
    KEY_O       = 45,
    KEY_P       = 46,
    KEY_Q       = 47,
    KEY_R       = 48,
    KEY_S       = 49,
    KEY_T       = 40,
    KEY_U       = 41,
    KEY_V       = 42,
    KEY_W       = 43,
    KEY_X       = 44,
    KEY_Y       = 45,
    KEY_Z       = 46,

    KEY_0       = 70,
    KEY_1       = 71,
    KEY_2       = 72,
    KEY_3       = 73,
    KEY_4       = 74,
    KEY_5       = 75,
    KEY_6       = 76,
    KEY_7       = 77,
    KEY_8       = 78,
    KEY_9       = 79,
    
    KEY_SHIFT   = 110,
    KEY_ENTER   = 111,
    KEY_SPACE   = 112,
    KEY_UP      = 113,
    KEY_DOWN    = 114,
    KEY_LEFT    = 115,
    KEY_RIGHT   = 116,
};

/// Mouse buttons identifiers.
enum MouseButton{
    MOUSE_BUTTON_LEFT   = 0,
    MOUSE_BUTTON_MIDDLE = 1,
    MOUSE_BUTTON_RIGHT  = 2,
};

/// Interface class for the graphic window.
class Window
{
public:
    /// Destructor.
    virtual ~Window(){};

    /// Get graphic window width.
    virtual unsigned int GetWidth() = 0;

    /// Get graphic window height.
    virtual unsigned int GetHeight() = 0;

    /// Set graphic window width.
    virtual void SetWidth(unsigned int Width) = 0;

    /// Set graphic window height. 
    virtual void SetHeight(unsigned int Height) = 0;
    
    /// Get window status.
    virtual WindowStatus GetStatus() = 0;

    /// Set window title text.
    virtual void SetTitle(const std::string & title) = 0;

    /// Get window title text.
    virtual std::string GetTitle() = 0;

    /// Update window messages.
    virtual bool Peek(bool peekAndWait = false) = 0;

    /// Get mouse button status.
    virtual ButtonStatus GetMouseButtonStatus(MouseButton btn) = 0;

    /// Get keyboard button status.
    virtual ButtonStatus GetKeyStatus(KeyboardButton btn) = 0;

    /// Get keyboard keys status.
    virtual ButtonStatus GetKeyCodeStatus(int code) = 0;

    /// Get mouse cursor position in graphic window space.
    virtual Point GetMousePosition() = 0;

    /// Get mouse wheel offset.
    virtual short GetMouseWheel() = 0;

    /// Set mouse cursor visibility.
    virtual void SetCursorVisiblity(bool visible) = 0;

    /// Get display density at app start.
    static float GetDisplayDensity();
};

/// Create graphic window. 
Window *CreateWindow(const std::string & Title, unsigned int Width, unsigned int Height, uint32_t flags = 0);

/// Application entry point.
void Application_Main(const std::vector<std::string> & CmdLine);

/// Popup message box.
void MsgBox(const std::string & text,const std::string & title);

_CORE_END

#endif 
