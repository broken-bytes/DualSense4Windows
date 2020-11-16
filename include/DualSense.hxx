#pragma once
#include <functional>
#include <thread>
#include <hidapi/hidapi.h>

namespace BrokenBytes::ControllerKit {
	class DualSense {
	public:
		struct DS_REPORT {
			char bThumbLX;
			char bThumbLY;
			char bThumbRX;
			char bThumbRY;
			uint16_t wButtons;
			char bSpecial;
			char bTriggerL;
			char bTriggerR;
		};
		/// <summary>
		/// Creates a new DS4 instance based on HID path
		/// </summary>
		/// <param name="path"></param>
		DualSense(char* path);
		/// <summary>
		/// Sets the callback to trigger on new report data
		/// </summary>
		/// <param name="callback"></param>
		void SetCallback(std::function<void(DS_REPORT)> callback);
		
	private:
		hid_device* _device;
		unsigned char _buffer[32];
		std::thread _thread;
		std::function<void(DS_REPORT)> _callback;
		
		void Routine();
		void WriteReports();
		void ReadReports();
	};
}
