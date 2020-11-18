#include "Core.hxx"
#include <CommCtrl.h>
#include <cstdint>

#include "MainWindow.hxx"

namespace BrokenBytes::DualSense4Windows::UI {

	constexpr wchar_t* MAIN_WINDOW = TEXT("MAIN_WINDOW");
	constexpr uint16_t WIDTH = 800;
	constexpr uint16_t HEIGHT = 800;


	MainWindow::MainWindow(
		LPCWSTR title,
		uint16_t width,
		uint16_t height
	) : Window(title, width, height) {
		HWND hWndComboBox = CreateWindowEx(
			WS_EX_WINDOWEDGE,
			TEXT("Button"),
			TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			0,
			0,
			256,
			32,
			Handle(),
			NULL,
			GetModuleHandle(nullptr),
			NULL
		);
	}
	

	void MainWindow::Show() {
		Window::Show();
	}
	
	void MainWindow::Hide() {
		Window::Hide();
	}
	
	LRESULT CALLBACK MainWindow::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if(uMsg == WM_DESTROY) {
			PostQuitMessage(0);
		}
		return DefWindowProc(Handle(), uMsg, wParam, lParam);
	}
}
