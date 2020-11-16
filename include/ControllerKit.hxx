#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <ViGEm/Client.h>

namespace BrokenBytes::ControllerKit {
	// Forward declarations
	class DualSense;
	struct DS_REPORT;

	// Types
	enum class ControllerMode {
		DS4,
		XBoxOne
	};

	enum class DPad {
		None,
		Left,
		LeftUp,
		Up,
		RightUp,
		Right,
		RightDown,
		Down,
		LeftDown
	};

	// Hardware IDs
	constexpr uint16_t SONY   = 0x054C;
	constexpr uint16_t DS   = 0x0CE6;
	constexpr int8_t DS_INTERFACE = 0x03;

	// Device Section
	extern PVIGEM_CLIENT CLIENT;
	extern std::map<char*, DualSense*> DEVICES;
	extern std::map<char*, PVIGEM_TARGET> VIRTUAL_DEVICES;
	constexpr ControllerMode DEFAULT_MODE = ControllerMode::XBoxOne;
	
	// UI Section
	constexpr wchar_t* WINDOW_NAME = L"DualSense4Windows";

	/// <summary>
	/// Inits the program and does initial setup needed
	/// </summary>
	void InitApp();
	
	/// <summary>
	/// Creates the ViGem instance for virtualization
	/// </summary>
	void InitViGEmClient();
	/// <summary>
	/// Gets all DualSense devices connected
	/// </summary>
	/// <returns>A map of devices, by HID path and instance </returns>
	std::map<char*, DualSense*> GetDualSenses();
	/// <summary>
	/// Updates the list of connected DualSenses
	/// </summary>
	void UpdateDualSenseDevices();

	/// <summary>
	/// Creates a new virtual device with type
	/// </summary>
	/// <param name="mode"></param>
	/// <returns>The virtual device</returns>
	PVIGEM_TARGET CreateVirtualDevice(ControllerMode mode);

	/// <summary>
	/// Removes a virtual device from the bus
	/// </summary>
	/// <param name="">The device to remove</param>
	void RemoveVirtualDevice(PVIGEM_TARGET device);

	/// <summary>
	/// Binds a virtual device to a physical DualSense
	/// </summary>
	/// <param name="device">The device to bind</param>
	/// <param name="target">The target to be bound to</param>
	void BindVirtualDevice(DualSense* device, PVIGEM_TARGET target);
}
