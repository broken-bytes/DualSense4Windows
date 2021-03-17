#pragma once

#include <map>
#include <string>
#include <memory>
#include <array>
#include <sigslot/signal.hpp>

#include <Qt/QtWidgets/QMainWindow>
#include <Qt/QtWidgets/QGridLayout>

#include "ColorPicker.hxx"
#include "Core.hxx"

#include "DualSense.hxx"
#include "DualSenseEntryWidget.hxx"
#include "DualSenseList.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	constexpr wchar_t* WINDOW_NAME = L"DualSense4Windows";
	constexpr uint32_t WINDOW_WIDTH = 800;
	constexpr uint32_t WINDOW_HEIGHT = 600;
	
	class MainWindow : public QMainWindow {
    public:
		/// <summary>
		/// Called when the MainWindow starts
		/// </summary>
		SIGNAL<> MainWindowStarted;
		/// <summary>
		/// Called when the MainWindow receives a device change event
		/// </summary>
		SIGNAL<std::vector<char*>> DevicesChanged;

		/// <summary>
		/// Called when the MainWindow triggers a color picker change
		/// </summary>
		SIGNAL<uint8_t, Color> ColorChanged;

		/// <summary>
		/// Creates an MainWindow instance
		/// </summary>
		MainWindow();
	    ~MainWindow();
	
		/// <summary>
		/// Runs the MainWindow loop
		/// </summary>
		void Run();

	    /// <summary>
	    /// Called on Update
	    /// </summary>
	    void OnUpdate();

	    /// <summary>
	    /// Called on close of the MainWindow
	    /// </summary>
	    void OnClose();
	    
		// Slots
		/// <summary>
		/// Called when the number of DualSense devices connected changes
		/// </summary>
		/// <param name="devices"></param>
		SLOT DualSenseDevicesChanged(std::vector<char*> devices);


	private:
		std::shared_ptr<UI::Window> _mainWindow;
		std::shared_ptr<UI::Window> _window;
		std::unique_ptr<QGridLayout> _layout;
		std::unique_ptr<DualSenseList> _list;
		
		void RegisterWindows();
    }; 
}
