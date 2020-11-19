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
		sigslot::signal<> DevicesChanged;

		/// <summary>
		/// Called when the app triggers a color picker change
		/// </summary>
		sigslot::signal<uint8_t, Color> ColorChanged;

		/// <summary>
		/// Creates an app instance
		/// </summary>
		App();
	    ~App();
	
		/// <summary>
		/// The currently window that is active
		/// </summary>
		/// <returns>The window handle</returns>
		std::shared_ptr<UI::Window> Window() const;

		/// <summary>
		/// Runs the app loop
		/// </summary>
		void Run();

	    /// <summary>
	    /// Called on Update
	    /// </summary>
	    void OnUpdate();

	    /// <summary>
	    /// Called on close of the App
	    /// </summary>
	    void OnClose();
	    
		// Slots
		/// <summary>
		/// Called when the number of DualSense devices connected changes
		/// </summary>
		/// <param name="devices"></param>
		void DualSenseDevicesChanged(std::vector<char*> devices);


	private:
		std::shared_ptr<UI::Window> _mainWindow;
		std::shared_ptr<UI::Window> _window;
		
		void RegisterWindows();
    }; 
}
