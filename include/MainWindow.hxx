#pragma once

#include <map>
#include <vector>
#include <sigslot/signal.hpp>

#include "ColorPicker.hxx"
#include "DualSense.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	
	enum class Dimensions : uint16_t {
		VSpacer = 1,
		HSpacer = 1,
		NameWidth = 30,
		ConnectionWidth = 10,
		ColorSize = 15,
		HideWidth = 10,
		BaseHeight = 5
	};
	
	class MainWindow : public Window {
	public:
		sigslot::signal<> DevicesChanged;
		sigslot::signal<uint8_t, Color> ColorChanged;
		
		/// <summary>
		/// Creates a new instance of this window
		/// </summary>
		/// <param name="width">The width of the window</param>
		/// <param name="height">The height of the window</param>
		MainWindow(LPCWSTR title, uint16_t width, uint16_t height);
		
		void Show() override;
		void Hide() override;

		/// <summary>
		/// Changes the device list of this window
		/// </summary>
		/// <param name="devices">The devices</param>
		void DualSenseDevicesChanged(std::vector<char*> devices);

	protected:
		LRESULT CALLBACK ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	private:
		std::vector<char*> _devices;
		std::map<char*, std::array<void*, 4>> _controls;

		/// <summary>
		/// Adds the controls for a given DualSense
		/// </summary>
		/// <param name="id">The path of the DualSense</param>
		void AddControls(char* id);

		/// <summary>
		/// Removes the controls for a given DualSense
		/// </summary>
		/// <param name="id">The path of the DualSense</param>
		void RemoveControls(char* id);
	};
}
