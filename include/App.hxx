#pragma once

#include <map>
#include <string>
#include <memory>
#include <array>
#include <sigslot/signal.hpp>


#include "ColorPicker.hxx"
#include "Core.hxx"

#include "DualSense.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	constexpr wchar_t* WINDOW_NAME = L"DualSense4Windows";
	constexpr uint32_t WINDOW_WIDTH = 800;
	constexpr uint32_t WINDOW_HEIGHT = 600;
	
	class App {

    public:
		/// <summary>
		/// Called when the app starts
		/// </summary>
		sigslot::signal<> AppStarted;
		/// <summary>
		/// Called when the app receives a device change event
		/// </summary>
		sigslot::signal<> DeviceChange;

		sigslot::signal<uint8_t, Color> ColorChanged;

		/// <summary>
		/// Creates an app instance
		/// </summary>
		/// <param name="instance">The Win32 App instance</param>
		App(HINSTANCE instance);
	    ~App();
	
		/// <summary>
		/// The currently window that is active
		/// </summary>
		/// <returns>The window handle</returns>
		std::shared_ptr<UI::Window> Window() const;
		
		void Run();
	    void OnUpdate();
	    void OnClose();
	    
		// Slots
		/// <summary>
		/// Called when the number of DualSense devices connected changes
		/// </summary>
		/// <param name="devices"></param>
		void DualSenseDevicesChanged(std::map<char*, DualSense*> devices);


	private:
		HINSTANCE _win32Instance;
		std::shared_ptr<UI::Window> _mainWindow;
		std::shared_ptr<UI::Window> _window;
		
		void RegisterWindows();
    }; 
}
