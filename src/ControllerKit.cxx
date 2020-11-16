#ifndef UNICODE
#define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <map>

#include <hidapi/hidapi.h>
#include <ViGEm/Client.h>

#include "ControllerKit.hxx"
#include "DualSense.hxx"
#include "Utils.hxx"


using namespace BrokenBytes::ControllerKit;

namespace BrokenBytes::ControllerKit {
	PVIGEM_CLIENT CLIENT;
	std::map<char*, DualSense*> DEVICES;
	std::map<char*, PVIGEM_TARGET> VIRTUAL_DEVICES;


	/// <summary>
	/// Callback used for ViGEm notification when in DS4 Mode.
	/// </summary>
	/// <param name="Client"></param>
	/// <param name="Target"></param>
	/// <param name="LargeMotor"></param>
	/// <param name="SmallMotor"></param>
	/// <param name="LightbarColor"></param>
	/// <param name="UserData"></param>
	static void __stdcall DS4_Notification(
		PVIGEM_CLIENT Client,
		PVIGEM_TARGET Target,
		UCHAR LargeMotor,
		UCHAR SmallMotor,
		DS4_LIGHTBAR_COLOR LightbarColor,
		LPVOID UserData
	) {
		auto device = static_cast<DualSense*>(UserData);
		device->SetLEDColor(
			DS_LIGHTBARCOLOR{
				LightbarColor.Red,
				LightbarColor.Green,
				LightbarColor.Blue
			}
		);
		device->SetRumble(LargeMotor, DS_RUMBLE::Heavy);
		device->SetRumble(SmallMotor, DS_RUMBLE::Light);
	}

/// <summary>
/// Callback used for ViGEm notification when in XBOX Mode.
/// </summary>
/// <param name="Client"></param>
/// <param name="Target"></param>
/// <param name="LargeMotor"></param>
/// <param name="SmallMotor"></param>
/// <param name="LedNumber"></param>
/// <param name="UserData"></param>
static void __stdcall XBOX_Notification(
	PVIGEM_CLIENT Client,
	PVIGEM_TARGET Target,
	UCHAR LargeMotor,
	UCHAR SmallMotor,
	UCHAR LedNumber,
	LPVOID UserData
) {
	auto device = static_cast<DualSense*>(UserData);
	device->SetRumble(LargeMotor, DS_RUMBLE::Heavy);
	device->SetRumble(SmallMotor, DS_RUMBLE::Light);
	if (auto d = device; d->Target() == Target) {
		return;
	}
	device->SetLEDColor(DS_LIGHTBARCOLOR{ 255, 255,0 });
	}


void InitApp() {
	InitViGEmClient();
	DEVICES = std::map<char*, DualSense*>();
	VIRTUAL_DEVICES = std::map<char*, PVIGEM_TARGET>();
	auto dualsenses = GetDualSenses();
	for (auto ds : dualsenses) {
		auto key = ds.first;
		auto value = ds.second;
		const auto device = CreateVirtualDevice(DEFAULT_MODE);
		VIRTUAL_DEVICES.emplace(ds.first, device);
		BindVirtualDevice(ds.second, device);
	}
}

void InitViGEmClient() {
	CLIENT = vigem_alloc();
	if (CLIENT == nullptr)
	{
		std::cerr << "Uh, not enough memory to do that?!" << std::endl;
		return;
	}
	if (const auto code = vigem_connect(CLIENT); !VIGEM_SUCCESS(code))
	{
		std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << code << std::endl;
		return;
	}
}

std::map<char*, DualSense*> GetDualSenses() {
	auto devices = std::map<char*, DualSense*>();

	auto* ds = hid_enumerate(SONY, DS);
	while (ds != nullptr) {
		if (ds->interface_number != DS_INTERFACE) {
			ds = ds->next;
			continue;
		}
		devices.emplace(
			ds->path,
			new DualSense(ds->path, devices.size())
		);
		ds = ds->next;
	}
	hid_free_enumeration(ds);

	return devices;
}

void UpdateDualSenseDevices() {
	auto dualsenses = GetDualSenses();

	// Check if a device is no longer present
	for (auto ds : DEVICES) {
		for (auto d : dualsenses) {
			if (d.first == ds.first) {
				break;
			}
		}
		DEVICES.erase(ds.first);
		RemoveVirtualDevice(VIRTUAL_DEVICES[ds.first]);
	}

	// Check if a new device has been connected
	for (auto ds : dualsenses) {
		if (DEVICES.count(ds.first) == 0) {
			DEVICES.emplace(ds.first, ds.second);
			BindVirtualDevice(ds.second, CreateVirtualDevice(DEFAULT_MODE));
			ds.second->SetLEDColor(DS_LIGHTBARCOLOR{ 255, 120, 120 });
		}
	}
}

PVIGEM_TARGET CreateVirtualDevice(ControllerMode mode) {
	PVIGEM_TARGET target = nullptr;
	switch (mode) {
	case ControllerMode::DS4:
		target = vigem_target_ds4_alloc();
		break;
	case ControllerMode::XBoxOne:
		target = vigem_target_x360_alloc();
		break;
	}
	if (auto code = vigem_target_add(CLIENT, target); !VIGEM_SUCCESS(code))
	{
		throw std::runtime_error("Could not add target");
		return nullptr;
	}
	return target;
}

void RemoveVirtualDevice(PVIGEM_TARGET device) {
	vigem_target_remove(CLIENT, device);
	vigem_target_free(device);
}

void BindVirtualDevice(DualSense* device, PVIGEM_TARGET target) {
	if (device->Mode() == ControllerMode::XBoxOne) {
		auto path = device->Path();
		auto code = vigem_target_x360_register_notification(
			CLIENT,
			target,
			&XBOX_Notification,
			device);
		if (!VIGEM_SUCCESS(code)) {
			throw std::runtime_error("Failed to add callback");
		}
	}
	else {
		auto path = device->Path();
		auto code = vigem_target_ds4_register_notification(
			CLIENT,
			target,
			&DS4_Notification,
			device);
		if (!VIGEM_SUCCESS(code)) {
			throw std::runtime_error("Failed to add callback");
		}
	}

	device->SetModeCallback([device, target](ControllerMode mode) {
		VIRTUAL_DEVICES.erase(device->Path());
		const auto newTarget = CreateVirtualDevice(mode);

		VIRTUAL_DEVICES.emplace(device->Path(), newTarget);
		BindVirtualDevice(device, newTarget);
		});
	device->SetReportCallback([device, target](DS_REPORT raw) {
		switch (device->Mode()) {
		case ControllerMode::DS4:
			DS4_REPORT ds4Report;
			Utils::MapDS4Report(ds4Report, raw);
			vigem_target_ds4_update(CLIENT, target, ds4Report);
			break;
		case ControllerMode::XBoxOne:
			XUSB_REPORT xusbReport;
			Utils::MapXUSBReport(xusbReport, raw);
			vigem_target_x360_update(CLIENT, target, xusbReport);
			break;
		}
		});
}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	InitApp();
	
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		WINDOW_NAME,                     // Window class
		WINDOW_NAME,    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);



		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
