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
	) : Window(title, width, height, WS_VSCROLL) {
	}
	
	void MainWindow::Show() {
		Window::Show();
		auto color = new ColorPicker(Color { 120, 120, 120});
		color->ColorChanged.connect([this](Color c) {
			ColorChanged(0, c);
		});
		color->Show();
	}
	
	void MainWindow::Hide() {
		Window::Hide();
	}

	void MainWindow::DualSenseDevicesChanged(std::vector<char*> devices) {
		// Remove removed devices

		if(_devices.empty()) {
			for(auto d: devices) {
				_devices = devices;
				AddControls(d);
			}
			return;
		}
		
		for (int x = 0; x < _devices.size(); x++) {
			const auto found = std::find(
				_devices.begin(),
				_devices.end(),
				devices[x]
			) != _devices.end();
			if (!found) {
				_devices.erase(_devices.begin() + x);
				RemoveControls(devices[x]);
			}
		}
		
		// Add new devices
		for (int x = 0; x < _devices.size(); x++) {
			const auto found = std::find(
				_devices.begin(),
				_devices.end(),
				devices[x]
			) != _devices.end();
			if (!found) {
				_devices.erase(_devices.begin() + x);
				AddControls(devices[x]);
			}
		}

		UpdateWindow(Handle());
	}

	LRESULT CALLBACK MainWindow::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		if(uMsg == WM_DESTROY) {
			PostQuitMessage(0);
		}

		if(uMsg == WM_DEVICECHANGE) {
			DevicesChanged();
		}

		if (uMsg == WM_PAINT) {
			hdc = BeginPaint(Handle(), &ps);
			uint16_t hSpacer = Width() *
				(static_cast<uint16_t>(Dimensions::HSpacer) / 100.0);
			uint16_t vSpacer = Height() *
				(static_cast<uint16_t>(Dimensions::VSpacer) / 100.0);
			uint16_t textWidth = Width() *
				(static_cast<uint16_t>(Dimensions::NameWidth) / 100.0);
			uint16_t textHeight = Height() *
				(static_cast<uint16_t>(Dimensions::BaseHeight) / 100.0);
			for (int x = 0; x < _controls.size(); x++) {
				RECT r{ hSpacer, vSpacer, textWidth, textHeight };
				TextOut(hdc, r.left, r.top, L"DualSense", 9);
				//DrawText(hdc, L"DualSense", 9, &r, DT_CENTER | DT_LEFT);
			}
		}
		
		return DefWindowProc(Handle(), uMsg, wParam, lParam);
	}

	void MainWindow::AddControls(char* id) {
		_controls.emplace(id, std::array<void*, 4>());
	}
	
	void MainWindow::RemoveControls(char* id) {}
}
