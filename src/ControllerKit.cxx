#include <map>
#include <memory>

#include "ControllerKit.hxx"

#include "App.hxx"
#include "Utils.hxx"
#include "Interface.hxx"


using namespace BrokenBytes::DualSense4Windows;

namespace BrokenBytes::DualSense4Windows {
	std::unique_ptr<Interface> INTERFACE;
	std::unique_ptr<UI::App> APP;

	std::unique_ptr<HINSTANCE> INSTANCE;
	
	void Init() {
		InitInterface();
		InitUI();
		ConnectSignals();
	}

	void InitUI() {
		APP = std::make_unique<UI::App>();
	}

	void InitInterface() {
		INTERFACE = std::make_unique<Interface>();
	}

	void ConnectSignals() {
		APP->AppStarted.connect([] {
			INTERFACE->Init();
		});
		APP->DevicesChanged.connect([] {
			INTERFACE->UpdateDualSenseDevices();
		});
		INTERFACE->DevicesChanged.connect([](
			std::vector<char*> devices
			) {
				APP->DualSenseDevicesChanged(devices);
		});
		APP->ColorChanged.connect([](uint8_t id, UI::Color c) {
			INTERFACE->SetColor(id, DS_LIGHTBARCOLOR{c.R, c.G, c.B});
		});
	}

	void Run() {
		APP->Run();
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	INSTANCE = std::make_unique<HINSTANCE>(hInstance);
	Init();
	Run();
	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		APP->OnUpdate();
	}

	return 0;
}
