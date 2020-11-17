#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ViGEm/Client.h>
#include <bitset>

#include "Utils.hxx"
#include "DualSense.hxx"
#include "Types.hxx"

namespace BrokenBytes::DualSense4Windows::Utils {
	void MapDS4Report(DS4_REPORT& report, DS_REPORT raw) {
		DS4_REPORT_INIT(&report);
		report.bThumbLX = raw.bThumbLX;
		report.bThumbLY = raw.bThumbLY;
		report.bThumbRX = raw.bThumbRX;
		report.bThumbRY = raw.bThumbRY;
		report.wButtons = raw.wButtons;
		report.bTriggerL = raw.bTriggerL;
		report.bTriggerR = raw.bTriggerR;
		report.bSpecial = raw.bSpecial;
	}

	void MapXUSBReport(XUSB_REPORT& report, DS_REPORT raw) {
		// Button Layout
		XUSB_REPORT_INIT(&report);
		std::bitset<16> buttons;
		std::bitset<4> dpad;

		DS_INPUT input;
		GetDSInputFromReport(raw, input);


		switch (input.DPad) {
		case DPad::Down:
			dpad[1] = true;
		case DPad::Left:
			dpad[2] = true;
			break;
		case DPad::None:
			break;
		case DPad::LeftUp:
			dpad[0] = true;
			dpad[2] = true;
			break;
		case DPad::Up:
			dpad[0] = true;
			break;
		case DPad::RightUp:
			dpad[0] = true;
			dpad[3] = true;
			break;
		case DPad::Right:
			dpad[3] = true;
			break;
		case DPad::RightDown:
			dpad[1] = true;
			dpad[3] = true;
			break;
		case DPad::LeftDown:
			dpad[1] = true;
			dpad[2] = true;
			break;
		}

		for (int x = 0; x < 4; x++) {
			buttons[x] = dpad[x];
		}

		buttons[4] = input.Options;
		buttons[5] = input.Create;
		buttons[6] = input.L3;
		buttons[7] = input.R3;
		buttons[8] = input.L1;
		buttons[9] = input.R1;
		buttons[10] = input.PS;
		buttons[11] = false;
		buttons[12] = input.Cross;
		buttons[13] = input.Circle;
		buttons[14] = input.Square;
		buttons[15] = input.Triangle;

		report.wButtons = static_cast<uint16_t>(buttons.to_ulong());
		report.bLeftTrigger = raw.bTriggerL;
		report.bRightTrigger = raw.bTriggerR;
		report.sThumbLX = (((raw.bThumbLX) * 255) - 32768);
		report.sThumbLY = ~(((raw.bThumbLY) * 255) - 32768);
		report.sThumbRX = (((raw.bThumbRX) * 255) - 32768);
		report.sThumbRY = ~(((raw.bThumbRY) * 255) - 32768);
	}

	void GetDSInputFromReport(DS_REPORT report, DS_INPUT& input) {
		input.L2 = false;
		input.R2 = false;
		input.Triangle = (report.wButtons & 0x80);
		input.Circle = (report.wButtons & 0x40);
		input.Cross = (report.wButtons & 0x20);
		input.Square = (report.wButtons & 0x10);
		input.L1 = (report.wButtons & 0x100);
		input.R1 = (report.wButtons & 0x200);
		input.Create = (report.wButtons & 0x1000);
		input.Options = (report.wButtons & 0x2000);
		input.L3 = (report.wButtons & 0x4000);
		input.R3 = (report.wButtons & 0x8000);
		input.PS = (report.bSpecial & 0x01);

		switch (report.wButtons & 0x000F) {
		case 0b0000:
			input.DPad = DPad::Up;
			break;
		case 0b0001:
			input.DPad = DPad::RightUp;
			break;
		case 0b0010:
			input.DPad = DPad::Right;
			break;
		case 0b0011:
			input.DPad = DPad::RightDown;
			break;
		case 0b0100:
			input.DPad = DPad::Down;
			break;
		case 0b0101:
			input.DPad = DPad::LeftDown;
			break;
		case 0b0110:
			input.DPad = DPad::Left;
			break;
		case 0b0111:
			input.DPad = DPad::LeftUp;
			break;
		case 0b1000:
			input.DPad = DPad::None;
			break;
		}
	}
}
