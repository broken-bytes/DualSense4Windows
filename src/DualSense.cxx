#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>

#include <hidapi/hidapi.h>

#include "DualSense.hxx"
#include "Math.hxx"
#include "Utils.hxx"

uint8_t reportID = 0x00;
uint8_t protocolID = 0x00;

namespace BrokenBytes::DualSense4Windows {
	DualSense::DualSense(char* path, uint8_t controllerNumber) {
		_path = path;
		_device = hid_open_path(_path);
		_thread = std::thread(&DualSense::Routine, this);
		_controllerNumber = controllerNumber;
		_mute = 0;
		_triggerRight = {0,0,0, DS_TRIGGERMODE::Free};
		_triggerLeft = {0,0,0, DS_TRIGGERMODE::Free };
		_rumbleLight = 0;
		_rumbleHeavy = 0;
		_mode = DEFAULT_MODE;
	}

	DualSense::~DualSense() {
		hid_close(_device);
	}

	ControllerMode DualSense::Mode() const {
		return _mode;
	}

	char* DualSense::Path() const {
		return _path;
	}

	PVIGEM_TARGET DualSense::Target() const {
		return _target;
	}

	std::string DualSense::MAC() const {
		wchar_t* str = nullptr;
			
		if(hid_get_manufacturer_string(_device, str, 64) != 0) {
			str = L"";
		}

		auto err = hid_error(_device);
		wprintf(err);
		std::wstring ws(str);
		return std::string(ws.begin(), ws.end());
	}

	void DualSense::SetLEDColor(DS_LIGHTBARCOLOR color) {
		_color = color;
	}

	void DualSense::SetTrigger(DS_TRIGGER trigger, uint8_t triggerId) {
		if(triggerId > 0) {
			_triggerRight = trigger;
			return;
		}

		_triggerLeft = trigger;
	}

	void DualSense::SetRumble(uint8_t rumble, DS_RUMBLE type) {
		if(type == DS_RUMBLE::Heavy) {
			_rumbleHeavy = rumble;
			return;
		}
		_rumbleLight = rumble;
	}

	void DualSense::SetMute(uint8_t muted) {
		_mute = muted;
	}

	void DualSense::SetMode(ControllerMode mode) {
		if(mode == _mode) {
			return;
		}
		_mode = mode;
		_modeCallback(_mode);
	}

	void DualSense::SetTarget(PVIGEM_TARGET target) {
		_target = target;
	}

	void DualSense::SetReportCallback(std::function<void(DS_REPORT)> reportCallback) {
		this->_reportCallback = reportCallback;
	}

	void DualSense::SetModeCallback(std::function<void(ControllerMode)> modeCallback
	) {
		this->_modeCallback = modeCallback;
	}

	void DualSense::Routine() {
		uint8_t ticks = 0;
		memset(_buffer, 0, 32);
		while (_device != nullptr) {
			ticks++;
			ReadReports();
			if(ticks < DS_SENDRATE) {
				continue;
			} else {
				ticks = 0;
			}
			
			WriteReports();
		}
	}

	void DualSense::ReadReports() {		
		if(hid_read(_device, _buffer, 32) < 1) {
			return;
		}
		
		uint16_t btns = _buffer[8];
		btns += (_buffer[9] << 8);
		btns += (_buffer[10] << 16);
		
		const DS_REPORT rep = DS_REPORT{
			static_cast<char>(_buffer[1]),
			static_cast<char>(_buffer[2]),
			static_cast<char>(_buffer[3]),
			static_cast<char>(_buffer[4]),
			btns,
			static_cast<char>(_buffer[10]),
			static_cast<char>(_buffer[5]),
			static_cast<char>(_buffer[6])
		};
		
		if (_reportCallback != nullptr) {
			_reportCallback(rep);
		}
	}

	void DualSense::WriteReports() {
		unsigned char outputReport[48];
		memset(outputReport, 0, 48);
		// Config
		// WIP LEDs, Rumble
		// Not functional yet
		// Contains parts of https://www.reddit.com/r/gamedev/comments/jumvi5/dualsense_haptics_leds_and_more_hid_output_report/
		outputReport[0] = 0x02; // report type

		outputReport[1] = 0xff; // flags determining what changes this packet will perform
			// 0x01 ??? used by PS Remote Play on startup
			// 0x02 ???
			// 0x04 ??? used by PS Remote Play on startup
			// 0x08 ??? used by PS Remote Play on startup
			// 0x10 modification of audio volume
			// 0x20 toggling of internal speaker while headset is connected 
			// 0x40 modification of microphone volume
			// 0x80 toggling of internal mic or external speaker while headset is connected 
		outputReport[2] = 0x15; // further flags determining what changes this packet will perform
			// 0x01 toggling microphone LED
			// 0x02 toggling audio/mic mute
			// 0x04 toggling LED strips on the sides of the touchpad
			// 0x08 will actively turn all LEDs off? Convenience flag? (if so, third parties might not support it properly)
			// 0x10 toggling white player indicator LEDs below touchpad
			// 0x20 ???
			// 0x40 ??? used by PS Remote Play on startup and exit
			// 0x80 ???

		// main motors		
		outputReport[3] = _rumbleLight; // left low freq motor 0-255
		outputReport[4] = _rumbleHeavy; // right high freq motor 0-255

		// audio settings requiring volume control flags
		outputReport[5] = 0xff; // audio volume of connected headphones (maxes out at about 0x7f)
		outputReport[6] = 0xff; // volume of internal speaker (0-255) (ties in with index 38?!?)
		outputReport[7] = 0xff; // internal microphone volume (not at all linear; 0-255, appears to max out at about 64; 0 is not fully muted, use audio mute flag instead!)
		outputReport[8] = 0x0c; // internal device enablement override flags (0xc default by ps remote play)
			// 0x01 = enable internal microphone (in addition to a connected headset) 
			// 0x04 = ??? set by default via PS Remote Play
			// 0x08 = ??? set by default via PS Remote Play 
			// 0x10 = disable attached headphones (only if 0x20 to enable internal speakers is provided as well)
			// 0x20 = enable audio on internal speaker (in addition to a connected headset)

		// audio related LEDs requiring according LED toggle flags
		outputReport[9] = _mute; // microphone LED (1 = on, 2 = pulsating / neither does affect the mic)

		// audio settings requiring mute toggling flags
		outputReport[10] = 0x00; // 0x10 microphone mute, 0x40 audio mute
		
		// left trigger motor  
		outputReport[11] = static_cast<uint8_t>(_triggerRight.Mode); // right trigger motor mode (0 = no resistance, 1 = continuous resistance, 2 = section resistance / PS Remote Play defaults this to 5; bit 4 only disables the motor?)
		outputReport[12] = _triggerRight.ResistanceStart; // right trigger start of resistance section (0-255; 0 = released state; 0xb0 roughly matches trigger value 0xff)
		outputReport[13] = _triggerRight.Resistance; // right trigger
			// (mode1) amount of force exerted; 0-255
			// (mode2) end of resistance section (>= begin of resistance section is enforced); 0xff makes it behave like mode1
		outputReport[14] = 0; // right trigger force exerted in range (mode2), 0-255

		outputReport[22] = static_cast<uint8_t>(_triggerLeft.Mode);// left trigger motor mode (0 = no resistance, 1 = continuous resistance, 2 = section resistance / PS Remote Play defaults this to 5; bit 4 only disables the motor?)
		outputReport[23] = _triggerLeft.ResistanceStart; // left trigger start of resistance section 0-255 (0 = released state; 0xb0 roughly matches trigger value 0xff)
		outputReport[24] = _triggerLeft.Resistance; // left trigger
			// (mode1) amount of force exerted; 0-255
			// (mode2) end of resistance section (>= begin of resistance section is enforced); 0xff makes it behave like mode1
		outputReport[25] = 0; // left trigger: (mode2) amount of force exerted within range; 0-255
		
		outputReport[38] = 0x07; // volume of internal speaker (0-7; ties in with index 6)

		// Uninterruptable Pulse LED setting (requires LED setting flag)
		outputReport[39] = 0; // 2 = blue LED pulse (together with index 42)
		outputReport[42] = 0; // pulse option
		/*1 = slowly(2s) fade to blue(scheduled to when the regular LED settings are active)
			2 = slowly(2s) fade out(scheduled after fade - in completion) with eventual switch back to configured LED color; only a fade - out can cancel the pulse(neither index 2, 0x08, nor turning this off will cancel it!)
			*/
			// Regular LED settings (requires LED setting flag)
		outputReport[44] = _controllerNumber + 1; // 5 white player indicator LEDs below the touchpad (bitmask 00-1f from left to right with 0x04 being the center LED)
		outputReport[45] = _color.R; // Red value of light bars left and right from touchpad
		outputReport[46] = _color.G; // Green value of light bars left and right from touchpad
		outputReport[47] = _color.B; // Blue value of light bars left and right from touchpad
		hid_write(_device, outputReport, 48);
	}
}
