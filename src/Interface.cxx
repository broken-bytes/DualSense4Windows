#include <iostream>
#include <memory>
#include <hidapi/hidapi.h>


#include "Interface.hxx"
#include "Types.hxx"
#include "DualSense.hxx"
#include "Utils.hxx"

namespace BrokenBytes::DualSense4Windows {
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
	
	Interface::Interface() {
		_client = nullptr;
	}

	void Interface::Init() {
		InitViGEmClient();
		_devices = std::map<char*, DualSense*>();
		_virtualDevices = std::map<char*, PVIGEM_TARGET>();
		auto dualsenses = GetDualSenses();
		for (auto ds : dualsenses) {
			auto key = ds.first;
			auto value = ds.second;
			const auto device = CreateVirtualDevice(DEFAULT_MODE);
			_virtualDevices.emplace(ds.first, device);
			BindVirtualDevice(ds.second, device);
			ds.second->SetLEDColor(DS_LIGHTBARCOLOR{ 255, 0 ,100 });
		}
		_devices = dualsenses;
		DevicesChanged(_devices);
	}

	void Interface::InitViGEmClient() {
		_client = vigem_alloc();
		if (_client == nullptr)
		{
			std::cerr << "Uh, not enough memory to do that?!" << std::endl;
			return;
		}
		if (const auto code = vigem_connect(_client); !VIGEM_SUCCESS(code))
		{
			std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << code << std::endl;
			return;
		}
	}

	std::map<char*, DualSense*> Interface::GetDualSenses() {
		auto _devices = std::map<char*, DualSense*>();

		auto* ds = hid_enumerate(SONY, DS);
		while (ds != nullptr) {
			if (ds->interface_number != DS_INTERFACE) {
				ds = ds->next;
				continue;
			}
			_devices.emplace(
				ds->path,
				new DualSense(ds->path, _devices.size())
			);
			ds = ds->next;
		}
		hid_free_enumeration(ds);

		return _devices;
	}

	void Interface::UpdateDualSenseDevices() {
		auto dualsenses = GetDualSenses();

		// Check if a device is no longer present
		for (auto ds : _devices) {
			for (auto d : dualsenses) {
				if (d.first == ds.first) {
					break;
				}
			}
			_devices.erase(ds.first);
			RemoveVirtualDevice(_virtualDevices[ds.first]);
		}

		// Check if a new device has been connected
		for (auto ds : dualsenses) {
			if (_devices.count(ds.first) == 0) {
				_devices.emplace(ds.first, ds.second);
				BindVirtualDevice(ds.second, CreateVirtualDevice(DEFAULT_MODE));
				ds.second->SetLEDColor(DS_LIGHTBARCOLOR{ 255, 120, 120 });
			}
		}
		DevicesChanged(_devices);
	}

	PVIGEM_TARGET Interface::CreateVirtualDevice(ControllerMode mode) {
		PVIGEM_TARGET target = nullptr;
		switch (mode) {
		case ControllerMode::DS4:
			target = vigem_target_ds4_alloc();
			break;
		case ControllerMode::XBoxOne:
			target = vigem_target_x360_alloc();
			break;
		}
		if (auto code = vigem_target_add(_client, target); !VIGEM_SUCCESS(code))
		{
			throw std::runtime_error("Could not add target");
			return nullptr;
		}
		return target;
	}

	void Interface::RemoveVirtualDevice(PVIGEM_TARGET device) {
		vigem_target_remove(_client, device);
		vigem_target_free(device);
	}

	void Interface::BindVirtualDevice(DualSense* device, PVIGEM_TARGET target) {
		if (device->Mode() == ControllerMode::XBoxOne) {
			auto path = device->Path();
			auto code = vigem_target_x360_register_notification(
				_client,
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
				_client,
				target,
				&DS4_Notification,
				device);
			if (!VIGEM_SUCCESS(code)) {
				throw std::runtime_error("Failed to add callback");
			}
		}

		device->SetModeCallback([this, device, target](ControllerMode mode) {
			_virtualDevices.erase(device->Path());
			const auto newTarget = CreateVirtualDevice(mode);

			_virtualDevices.emplace(device->Path(), newTarget);
			BindVirtualDevice(device, newTarget);
			});
		device->SetReportCallback([this, device, target](DS_REPORT raw) {
			switch (device->Mode()) {
			case ControllerMode::DS4:
				DS4_REPORT ds4Report;
				Utils::MapDS4Report(ds4Report, raw);
				vigem_target_ds4_update(_client, target, ds4Report);
				break;
			case ControllerMode::XBoxOne:
				XUSB_REPORT xusbReport;
				Utils::MapXUSBReport(xusbReport, raw);
				vigem_target_x360_update(_client, target, xusbReport);
				break;
			}
			});
	}
}
