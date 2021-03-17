#include "Core.hxx"
#include <CommCtrl.h>

#include "ColorPicker.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
    constexpr uint16_t WIDTH = 512;
    constexpr uint16_t HEIGHT = 250;

    constexpr uint8_t BTN_OK = 0;
    constexpr uint8_t BTN_CANCEL = 1;
	
    constexpr long COLOR_RANGE = MAKELONG(0, 255);
	ColorPicker::ColorPicker(Color initColor):
		Window(TEXT("Color Picker"), WIDTH, HEIGHT) {
        _color = initColor;
        _oldColor = initColor;
        _rH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("R"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE |
            TBS_ENABLESELRANGE,              // style 
            48, 8,                          // position 
            WIDTH - 64, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );
        auto label = CreateWindowEx(0, L"STATIC", L"R", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            0, 8, 48, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);

        SendMessage(_rH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));
        SendMessage(_rH, TBM_SETRANGE, true, COLOR_RANGE);

		
        _gH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("G"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE |
            TBS_ENABLESELRANGE,              // style 
            48, 40,                          // position 
            WIDTH - 64, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );

        label = CreateWindowEx(0, L"STATIC", L"G", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            0, 8, 48, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);
		
        SendMessage(_gH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));
        SendMessage(_gH, TBM_SETRANGE, true, COLOR_RANGE);
		
        _bH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("B"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE | 
            TBS_ENABLESELRANGE,              // style 
            48, 72,                          // position 
            WIDTH - 64, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );
        label = CreateWindowEx(0, L"STATIC", L"B", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            0, 8, 48, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);

        SendMessage(_bH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));
        SendMessage(_bH, TBM_SETRANGE, true, COLOR_RANGE);

        CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Apply",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            32,         // x position 
            172,         // y position 
            64,        // Button width
            24,        // Button height
            Handle(),     // Parent window
            reinterpret_cast<HMENU>(BTN_OK),       // No menu.
			reinterpret_cast<HINSTANCE>(GetWindowLongPtr(Handle(), GWLP_HINSTANCE)),
            NULL);
        CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Cancel",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            WIDTH - 96 - 32,         // x position 
            172,         // y position 
            64,        // Button width
            24,        // Button height
            Handle(),     // Parent window
            reinterpret_cast<HMENU>(BTN_CANCEL),       // No menu.
            reinterpret_cast<HINSTANCE>(GetWindowLongPtr(Handle(), GWLP_HINSTANCE)),
            NULL);	
		
        SetResizable(false);
	}

    ColorPicker::~ColorPicker() {
        DestroyWindow(Handle());
	}

    void ColorPicker::Show() {
        Window::Show();
        SetWindowPos(Handle(), HWND_TOPMOST, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}

    void ColorPicker::SetColor(uint8_t r, uint8_t g, uint8_t b) {
        SetColor(Color{ r, g, b });
	}
    void ColorPicker::SetColor(Color c) {
        _color = c;
        ColorChanged(c);
	}

    LRESULT ColorPicker::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        int wmId, wmEvent;
        PAINTSTRUCT ps;
        HDC hdc;
        RECT r { 0, 164, WIDTH, 104 };
        DWORD dwPos[3];
	    switch (uMsg) {
        case WM_PAINT:
            HBRUSH brush;
            hdc = BeginPaint(Handle(), &ps);
            brush = CreateSolidBrush(_color.RGB_VALUE());
            BeginPaint(Handle(), &ps);
            FillRect(hdc, &r, brush);
            DeleteObject(brush);
            DeleteObject(&r);
            EndPaint(Handle(), &ps);
	    	break;

        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_OK) {
                ColorChanged(_color);
                this->Hide();
                PostMessage(Handle(), WM_CLOSE, 0, 0);
            }
            if (LOWORD(wParam) == BTN_CANCEL) {
                ColorChanged(_oldColor);
                this->Hide();
                PostMessage(Handle(), WM_CLOSE, 0, 0);
            }
	    	break;
	    
        case WM_HSCROLL:
            dwPos[0] = SendMessage(_rH, TBM_GETPOS, 0, 0);
            dwPos[1] = SendMessage(_gH, TBM_GETPOS, 0, 0);
            dwPos[2] = SendMessage(_bH, TBM_GETPOS, 0, 0);
            SetColor(dwPos[0], dwPos[1], dwPos[2]);
            RedrawWindow(
                Handle(),
                nullptr,
                nullptr,
                RDW_INVALIDATE
            );
            break;

        case WM_CLOSE:
            this->~ColorPicker();
	    	break;
	    default:
            return DefWindowProcA(Handle(), uMsg, wParam, lParam);
	    }
        return 0;
	}

}
