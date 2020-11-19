#pragma once

#include <cstdint>
#include <string>
#include <Uxtheme.h>
#include <CommCtrl.h>
#include <WindowsX.h>
#include <Uxtheme.h>

#include "Core.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	constexpr LPCWSTR WINDOW_DEFAULT = L"WINDOW_DEFAULT";

	class Window {
	public:
		Window(
			LPCWSTR title,
			uint16_t width,
			uint16_t height
		) {
			_handle = nullptr;
			_isRegistered = false;
			WNDCLASSW wnd = { };
			wnd.hInstance = GetModuleHandle(nullptr);
			wnd.lpszClassName = WINDOW_DEFAULT;
			wnd.lpfnWndProc = WindowProcMain;
			wnd.style = CS_DBLCLKS | CS_PARENTDC;
			wnd.lpszMenuName = nullptr;
			Register(&wnd);
			_handle = CreateWindow(
				WINDOW_DEFAULT,
				title,
				WS_OVERLAPPEDWINDOW,
				0,
				0,
				width,
				height,
				nullptr,
				nullptr,
				GetModuleHandle(nullptr),
				nullptr
			);
			SendMessageW(_handle, WM_THEMECHANGED, 0, 0);
			SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			if (_handle == nullptr) {
				return;
			}
		}

		/// <summary>
		/// Wrapper WindowProc 
		/// </summary>
		static LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			auto ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
			auto w = reinterpret_cast<Window*>(ptr);
			
			if (w) {
				return w->ProcessEvent(uMsg, wParam, lParam);
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		
		/// <summary>
		/// Shows the window
		/// </summary>
		virtual void Show() {
			ShowWindow(_handle, SW_SHOW);
		}
		/// <summary>
		/// Hides the window
		/// </summary>
		virtual void Hide() {
			ShowWindow(_handle, SW_HIDE);
		}

		/// <summary>
		/// Toggles if this window can be resized
		/// </summary>
		/// <param name="resizable">Resizable state</param>
		void SetResizable(bool resizable) {
			if(resizable) {
				SetWindowLongPtr(
					_handle,
					GWL_STYLE,
					WS_OVERLAPPEDWINDOW
				);
				return;
			}
			SetWindowLongPtr(
				_handle,
				GWL_STYLE,
				WS_OVERLAPPED|
				WS_CAPTION |
				WS_SYSMENU |
				WS_MINIMIZEBOX |
				WS_MAXIMIZEBOX
			);
			
		}
		
	protected:
		/// <summary>
		/// Returns the internal handle
		/// </summary>
		/// <returns>The handle</returns>
		HWND Handle() const {
			return _handle;
		}

		/// <summary>
		/// Proccess a new event
		/// </summary>
		/// <param name="uMsg"></param>
		/// <param name="wParam"></param>
		/// <param name="lParam"></param>
		virtual LRESULT CALLBACK ProcessEvent(
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
		) {
			return DefWindowProc(Handle(), uMsg, wParam, lParam);
		}
		
		/// <summary>
		/// Register the class for this window
		/// </summary>
		/// <param name="wnd">The class</param>
		void Register(WNDCLASS* wnd) {
			if(_isRegistered) {
				return;
			}
			RegisterClass(wnd);
			_class = wnd;
			_isRegistered = true;
		}

		/// <summary>
		/// Sets the handle of this Window
		/// </summary>
		/// <param name="instance">The handle</param>
		void SetHandle(HWND instance) {
			_handle = instance;
		}

	private:
		HWND _handle;
		WNDCLASS* _class;
		bool _isRegistered;
	};
}
