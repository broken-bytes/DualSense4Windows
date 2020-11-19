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
	App::App() {
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

		auto m = reinterpret_cast<MainWindow*>(_mainWindow.get());
		m->ColorChanged.connect([this](
			uint8_t id, Color c) {
			this->ColorChanged(id, c);
		});

		m->DevicesChanged.connect([this] {
			DevicesChanged();
		});
		
		_mainWindow->Show();
		AppStarted();
	}

	void App::OnUpdate() {}
	void App::OnClose() {}

	void App::DualSenseDevicesChanged(std::vector<char*> devices) {
		reinterpret_cast<MainWindow*>(_mainWindow.get())->DualSenseDevicesChanged(devices);
	}
}


