#pragma once


#include <sigslot/signal.hpp>

#include "Core.hxx"
#include "Window.hxx"
#include "DualSenseInfo.hxx"

namespace BrokenBytes::DualSense4Windows::UI {	
	class ColorPicker : public Window {
	public:
		sigslot::signal<Color> ColorChanged;
		sigslot::signal<Color> ColorApplied;
		
		ColorPicker(Color initColor);
		virtual ~ColorPicker();

		void Show() override final;
		
	private:
		Color _oldColor;
		Color _color;
		HWND _rH;
		HWND _gH;
		HWND _bH;

		void SetColor(uint8_t r, uint8_t g, uint8_t b);
		void SetColor(Color c);
		LRESULT ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override final;
	};
}
