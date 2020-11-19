#pragma once

#include "Core.hxx"
#include <map>
#include <vector>
#include <ViGEm/Client.h>
#include <sigslot/signal.hpp>

#include "Types.hxx"

namespace BrokenBytes::DualSense4Windows {
	struct DS_LIGHTBARCOLOR;

	namespace UI {
		struct Color;
	}

	class DualSense;
	struct DS_REPORT;
}

namespace BrokenBytes::DualSense4Windows {
	// Hardware IDs
	constexpr uint16_t SONY = 0x054C;
	constexpr uint16_t DS = 0x0CE6;
	constexpr int8_t DS_INTERFACE = 0x03;

	class Interface {
	public:
		/// <summary>
		/// The signal used for device change notifications
		/// </summary>
		sigslot::signal < std::vector<char*>> DevicesChanged;


		/// <summary>
		/// Creates a new object
		/// Note: This does not init the interface
		/// </summary>
		Interface();
		/// <summary>
		/// Inits the Interface
		/// </summary>
		void Init();
		
		/// <summary>
		/// Gets all DualSense devices connected
		/// </summary>
		/// <returns>A map of devices, by HID path and instance </returns>
		std::vector<char*> GetDualSenses();
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

		void SetColor(uint8_t ID, DS_LIGHTBARCOLOR c);

		// Slots

	private:
		PVIGEM_CLIENT _client;
		std::map<char*, DualSense*> _devices;
		std::map<char*, PVIGEM_TARGET> _virtualDevices;
		
		void InitViGEmClient();
	};
}