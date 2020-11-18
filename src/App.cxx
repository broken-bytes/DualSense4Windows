#include <memory>
#include "App.hxx"

#include "ControllerKit.hxx"
#include "MainWindow.hxx"
#include "ColorPicker.hxx"
#include "Window.hxx"

#if !_DEBUG
#include "Assets.hxx"
#endif


namespace BrokenBytes::DualSense4Windows::UI {
	App::App(HINSTANCE instance) {
		_win32Instance = instance;
		RegisterWindows();
	}

	void App::RegisterWindows() {
	}

	App::~App() {
		
	}
	
	std::shared_ptr<Window> App::Window() const {
		return _window;
	}

	void App::Run() {
		_mainWindow = std::make_shared<MainWindow>(L"DualSense4Windows", 1024, 512);
		_mainWindow->Show();
		auto color = new ColorPicker();
		color->Show();
		AppStarted();
	}

	void App::OnUpdate() {}
	void App::OnClose() {}

	void App::DualSenseDevicesChanged(std::map<char*, DualSense*> devices) {
	
	}
}


