#include <iostream>
#include <memory>
#include <vector>
#include <hidapi/hidapi.h>

#include "Interface.hxx"
#include "ColorPicker.hxx"
#include "Types.hxx"
#include "DualSense.hxx"
#include "Utils.hxx"

namespace BrokenBytes::DualSense4Windows::IO {
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
	}
	
	Interface::Interface() : QObject(nullptr) {
		_client = nullptr;
	}

	void Interface::Init() {
		InitViGEmClient();
		_devices = std::map<char*, DualSense*>();
		_virtualDevices = std::map<char*, PVIGEM_TARGET>();
		UpdateDualSenseDevices();
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

	std::vector<char*> Interface::GetDualSenses() {
		auto devices = std::vector<char*>();

		auto* ds = hid_enumerate(SONY, DS);
		while (ds != nullptr) {
			if (ds->interface_number != DS_INTERFACE) {
				ds = ds->next;
				continue;
			}
			devices.emplace_back(ds->path);
			ds = ds->next;
		}
		hid_free_enumeration(ds);

		return devices;
	}

	void Interface::UpdateDualSenseDevices() {
		OutputDebugStringW(L"Updating Interface\n");
		auto dualsenses = GetDualSenses();
		for (auto& sense : dualsenses) {
			OutputDebugStringA(sense);
		}


		// Check if a device is no longer present
		for (auto ds : _devices) {
			bool remove = true;
			for (auto d : dualsenses) {
				if (strcmp(d, ds.first) == 0) {
					remove = false;
					break;
				} 
			}
			if (remove) {
				auto id = ds.first;
				delete ds.second;
				_devices.erase(ds.first);
				RemoveVirtualDevice(_virtualDevices[ds.first]);
			}
		}

		// Check if a new device has been connected
		for (auto ds : dualsenses) {
			bool add = true;
			for(auto d: _devices) {
				if (strcmp(ds, d.first) == 0) {
					add = false;
					continue;
				}
			}
			if(!add) {
				return;
			}
			
			auto dualsense = new DualSense(ds, 0);
			_devices.emplace(ds, dualsense);
			BindVirtualDevice(dualsense, CreateVirtualDevice(DEFAULT_MODE));
		}

		auto paths = std::vector<char*>();
		for(auto item: _devices) {
			paths.emplace_back(item.first);
		}
		DevicesChanged(paths);
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

	void Interface::SetColor(uint8_t ID, DS_LIGHTBARCOLOR c) {
		for (auto ds: _devices) {
			ds.second->SetLEDColor(c);
		}
	}
}
