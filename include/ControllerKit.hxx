#pragma once

#include <memory>

namespace BrokenBytes::DualSense4Windows {
	// Forward declarations
	namespace UI {
		class App;
	}

	class Interface;

	
	constexpr wchar_t* WINDOW_NAME = L"DualSense4Windows";
	extern std::shared_ptr<UI::App> APP;

	// Device Section
	extern std::shared_ptr<Interface> INTERFACE;
	
	/// <summary>
	/// Inits the program and does initial setup needed
	/// </summary>
	void Init();

	/// <summary>
	/// Inits the UI portion of the app
	/// </summary>
	void InitUI();

	/// <summary>
	/// Inits the interface for ViGEm and HID
	/// </summary>
	void InitInterface();
}
