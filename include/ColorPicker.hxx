#pragma once


#include "Core.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {

	struct Color {
		uint8_t R;
		uint8_t G;
		uint8_t B;

		COLORREF RGB_VALUE() const {
			return RGB(R, G, B);
		}
	};
	
	class ColorPicker : public Window {
	public:
		ColorPicker();

		void Show() override;
		
	private:
		Color _color;
		HWND _rH;
		HWND _gH;
		HWND _bH;

		void SetColor(uint8_t r, uint8_t g, uint8_t b);
		void SetColor(Color c);
		LRESULT ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};
}
