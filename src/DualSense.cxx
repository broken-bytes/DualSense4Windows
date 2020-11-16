#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <bitset>
#include <thread>

#include <hidapi/hidapi.h>

#include "../include/DualSense.hxx"
#include "../include/Math.hxx"

uint8_t reportID = 0x00;
uint8_t protocolID = 0x00;

namespace BrokenBytes::ControllerKit {
	DualSense::DualSense(char* path) {
		_device = hid_open_path(path);
		_thread = std::thread(&DualSense::Routine, this);	
	}
	
	void DualSense::SetCallback(std::function<void(DS_REPORT)> callback) {
		this->_callback = callback;
	}

	void DualSense::Routine() {
		memset(_buffer, 0, 32);
		while (_device != nullptr) {
			ReadReports();
			//WriteReports();
		}
	}

	void DualSense::ReadReports() {
		hid_set_nonblocking(_device, true);
		
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
		if (_callback != nullptr) {
			_callback(rep);
		}
	}

	void DualSense::WriteReports() {
		unsigned char report[52];
		memset(report, 0, 52);
		// Config
		// WIP LEDs, Rumble
		// Not functional yet
		hid_write(_device, report, 52);

		reportID++;
		if(reportID >= 254) {
			protocolID++;
		}

		std::cout << "Send " << reportID << " " << protocolID << std::endl;
	}
}
