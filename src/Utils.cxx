#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ViGEm/Client.h>

#include "Utils.hxx"

#include <bitset>
#include <iostream>


#include "DualSense.hxx"

namespace BrokenBytes::ControllerKit::Utils {
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

		DS_INPUT input;
		GetDSInputFromReport(raw, input);
		buttons[4] = input.Options;
		buttons[5] = input.Create;
		buttons[6] = input.L3;
		buttons[7] = input.R3;
		buttons[8] = input.L1;
		buttons[9] = input.R1;
		buttons[10] = input.PS;
		buttons[11] = input.Triangle;
		buttons[12] = input.Cross;
		buttons[13] = input.Circle;
		buttons[14] = input.Square;
		buttons[15] = false;

		report.wButtons = static_cast<uint16_t>(buttons.to_ulong());
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
	}
}
