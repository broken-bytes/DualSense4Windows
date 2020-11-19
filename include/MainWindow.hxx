#pragma once

#include <sigslot/signal.hpp>

#include "ColorPicker.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	class MainWindow : public Window {
	public:

		sigslot::signal<uint8_t, Color> ColorChanged;
		
		/// <summary>
		/// Creates a new instance of this window
		/// </summary>
		/// <param name="width">The width of the window</param>
		/// <param name="height">The height of the window</param>
		MainWindow(LPCWSTR title, uint16_t width, uint16_t height);
		
		void Show() override;
		void Hide() override;


		LRESULT CALLBACK ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};
}
