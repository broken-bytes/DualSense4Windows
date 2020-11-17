#pragma once

namespace BrokenBytes::DualSense4Windows {

	enum class ControllerMode {
		DS4,
		XBoxOne
	};

	enum class DPad {
		None,
		Left,
		LeftUp,
		Up,
		RightUp,
		Right,
		RightDown,
		Down,
		LeftDown
	};
}