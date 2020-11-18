#pragma once

#include <Logger.hxx>
#include <memory>
#include "Squirrel.hxx"

namespace BrokenBytes::DualSense4Windows {
	class Interface;

	
	constexpr wchar_t* WINDOW_NAME = L"DualSense4Windows";
	extern std::shared_ptr<Interface> INTERFACE;
	extern std::shared_ptr<Squirrel::Squirrel> LOGGER;

	
	/// <summary>
	/// Inits the program and does initial setup needed
	/// </summary>
	void Init();

	/// <summary>
	/// Inits the logger instance
	/// </summary>
	void InitLogger();

	/// <summary>
	/// Inits the UI portion of the app
	/// </summary>
	void InitUI();

	/// <summary>
	/// Inits the interface for ViGEm and HID
	/// </summary>
	void InitInterface();

	/// <summary>
	/// Connects all signals and slots
	/// </summary>
	void ConnectSignals();
	
	/// <summary>
	/// RUns the app loop
	/// </summary>
	void Run();

}
