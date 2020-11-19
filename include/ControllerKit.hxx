#pragma once
#include "Core.hxx"
#include <Logger.hxx>
#include <memory>
#include "Squirrel.hxx"
#include "App.hxx"


namespace BrokenBytes::DualSense4Windows {
	class Interface;

	extern std::unique_ptr<UI::App> APP;
	extern std::unique_ptr<Interface> INTERFACE;
	extern std::shared_ptr<Squirrel::Squirrel> LOGGER;
	extern std::unique_ptr<HINSTANCE> INSTANCE;
	
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
