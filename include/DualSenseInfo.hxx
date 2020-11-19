#pragma once
#include <string>
#include "Core.hxx"


namespace BrokenBytes::DualSense4Windows::UI {
	struct Color {
		uint8_t R;
		uint8_t G;
		uint8_t B;

		COLORREF RGB_VALUE() const {
			return RGB(R, G, B);
		}
	};
	
	enum class ConnectionType {
		USB,
		Bluetooth
	};

	enum class Mode {
		XBox,
		DS4,
		Native
	};
	
	struct DualSenseInfo {
		char* Path;
		std::wstring Name;
		ConnectionType Connection;
		Mode Mode;
		Color Color;
	};
}
