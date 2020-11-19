#pragma once

#include <map>
#include <vector>
#include <sigslot/signal.hpp>

#include "ColorPicker.hxx"
#include "DualSense.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	
	enum class Dimensions : uint16_t {
		VSpacer = 8,
		HSpacer = 1,
		NameWidth = 30,
		ConnectionWidth = 10,
		ColorSize = 15,
		HideWidth = 10,
		BaseHeight = 16
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
		std::vector<DualSenseInfo> _info;
		HWND _tabView;
		HWND _listView;
		std::map<std::wstring, HWND> _tabs;
		
		HWND CreateTabControl();
		HWND CreateListViewControl();
		void AddControls(char* id, int index);
		void RemoveControls(char* id);
	};
}
