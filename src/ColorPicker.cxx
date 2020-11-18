#include "Core.hxx"
#include <CommCtrl.h>

#include "ColorPicker.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
    constexpr uint16_t WIDTH = 512;
    constexpr uint16_t HEIGHT = 256;
	ColorPicker::ColorPicker() : Window(TEXT("Color Picker"), WIDTH, HEIGHT) {
        auto r = CreateWindowEx(
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

        SendMessage(r, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));

		
        auto g = CreateWindowEx(
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


        auto colorbar = CreateWindowEx(
            0,
            TEXT(""),
            TEXT(""),
            WS_CHILD | WS_VISIBLE,
            8,
            8,
            WIDTH - 8,
            HEIGHT - 8,
            Handle(),
            nullptr,
            GetModuleHandle(nullptr),
            nullptr
           );
		
        SendMessage(g, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));
		
        auto b = CreateWindowEx(
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

        SendMessage(b, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(label));

		
        SetResizable(false);
        SetWindowPos(Handle(), HWND_TOPMOST, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}

	void ColorPicker::Show() {
        Window::Show();
	}

    LRESULT ColorPicker::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        int wmId, wmEvent;
        PAINTSTRUCT ps;
        HDC hdc;
        RECT r { 0, 0, WIDTH, 32 };
	    switch (uMsg) {
        case WM_PAINT:
            HBRUSH brush;
            hdc = BeginPaint(Handle(), &ps);
            brush = reinterpret_cast<HBRUSH>(RGB(_color.R, _color.G, _color.B));
            BeginPaint(Handle(), &ps);
            FillRect(hdc, &r, brush);
            DeleteObject(brush);
            DeleteObject(&r);
            EndPaint(Handle(), &ps);
	    	break;
	    	default:
                return DefWindowProcA(Handle(), uMsg, wParam, lParam);
	    }
        return 0;
	}

}
