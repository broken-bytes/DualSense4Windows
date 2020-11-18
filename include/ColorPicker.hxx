#pragma once


#include "Core.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {

	struct Color {
		uint16_t R;
		uint16_t G;
		uint16_t B;
	};
	
	class ColorPicker : public Window {
	public:
		ColorPicker();

		void Show() override;

	private:
		Color _color;
	LRESULT ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};
}
