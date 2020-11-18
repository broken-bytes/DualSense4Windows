#include "Core.hxx"
#include <CommCtrl.h>

#include "ColorPicker.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
    constexpr uint16_t WIDTH = 512;
    constexpr uint16_t HEIGHT = 256;
	ColorPicker::ColorPicker() : Window(TEXT("Color Picker"), WIDTH, HEIGHT) {
        _rH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("R"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE |
            TBS_AUTOTICKS |
            TBS_ENABLESELRANGE,              // style 
            56, 8,                          // position 
            424, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );
        auto label = CreateWindowEx(0, L"STATIC", L"R", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            8, 8, 40, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);

        SendMessage(_rH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));

		
        _gH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("G"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE |
            TBS_AUTOTICKS |
            TBS_ENABLESELRANGE,              // style 
            56, 40,                          // position 
            424, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );

        label = CreateWindowEx(0, L"STATIC", L"G", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            8, 8, 40, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);
		
        SendMessage(_gH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));
		
        _bH = CreateWindowEx(
            0,                               // no extended styles 
            TRACKBAR_CLASS,                  // class name 
            TEXT("B"),              // title (caption) 
            WS_CHILD |
            WS_VISIBLE | 
            TBS_AUTOTICKS |
            TBS_ENABLESELRANGE,              // style 
            56, 72,                          // position 
            424, 24,                         // size 
            Handle(),                         // parent window 
            0,                     // control identifier 
            GetModuleHandle(nullptr),                         // instance 
            NULL                             // no WM_CREATE parameter 
        );
        label = CreateWindowEx(0, L"STATIC", L"B", SS_RIGHT | WS_CHILD | WS_VISIBLE,
            8, 8, 40, 16, Handle(), NULL, GetModuleHandle(nullptr), NULL);

        SendMessage(_bH, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));

		
        SetResizable(false);
        SetWindowPos(Handle(), HWND_TOPMOST, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}

	void ColorPicker::Show() {
        Window::Show();
	}

    void ColorPicker::SetColor(uint8_t r, uint8_t g, uint8_t b) {
        SetColor(Color{ r, g, b });
	}
    void ColorPicker::SetColor(Color c) {
        _color = c;
	}

    LRESULT ColorPicker::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        int wmId, wmEvent;
        PAINTSTRUCT ps;
        HDC hdc;
        RECT r { 0, 0, WIDTH, 32 };
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
	
        case TB_ENDTRACK:
            dwPos[0] = SendMessage(_rH, TBM_GETPOS, 0, 0) * 2;
            dwPos[1] = SendMessage(_gH, TBM_GETPOS, 0, 0) * 2;
            dwPos[2] = SendMessage(_bH, TBM_GETPOS, 0, 0) * 2;
            SetColor(dwPos[0], dwPos[1], dwPos[2]);
            RedrawWindow(Handle(), nullptr, nullptr, RDW_INVALIDATE);
            UpdateWindow(Handle());
            break;
	    default:
            return DefWindowProcA(Handle(), uMsg, wParam, lParam);
	    }
        return 0;
	}

}
