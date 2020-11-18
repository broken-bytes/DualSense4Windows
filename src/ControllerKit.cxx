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
		APP = std::make_unique<UI::App>(*INSTANCE);
	}

	void InitInterface() {
		INTERFACE = std::make_unique<Interface>();
	}

	void ConnectSignals() {
		APP->AppStarted.connect([] {
			INTERFACE->Init();
		});
		APP->DeviceChange.connect([] {
			INTERFACE->UpdateDualSenseDevices();
		});
		INTERFACE->DevicesChanged.connect([](
			std::map<char*, DualSense*> devices
			) {
				APP->DualSenseDevicesChanged(devices);
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
