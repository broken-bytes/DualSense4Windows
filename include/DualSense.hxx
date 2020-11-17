#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <functional>
#include <thread>
#include <hidapi/hidapi.h>
#include <ViGEm/Client.h>

#include "Types.hxx"


namespace BrokenBytes::DualSense4Windows {
	/// <summary>
	/// The rate at which to send reports to the DS
	/// </summary>
	constexpr uint8_t DS_SENDRATE = 50;
	/// <summary>
	/// The Default mode at which DualSense operates
	/// </summary>
	constexpr ControllerMode DEFAULT_MODE = ControllerMode::XBoxOne;

	struct DS_LIGHTBARCOLOR {
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	enum class DS_BUTTON {
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY,
		Square,
		Triangle,
		Circle,
		Cross,
		L1,
		R1,
		L2,
		R2,
		L3,
		R3,
		PS,
		Create,
		Options,
		Mute
	};

	enum class DS_RUMBLE {
		Heavy,
		Light
	}; 

	struct DS_INPUT {
		char LeftStickX;
		char LeftStickY;
		char RightStickX;
		char RightStickY;
		bool Square;
		bool Triangle;
		bool Circle;
		bool Cross;
		bool L1;
		bool R1;
		bool L2;
		bool R2;
		bool L3;
		bool R3;
		bool PS;
		bool Create;
		bool Options;
		bool Mute;
		DPad DPad;
		char LeftTrigger;
		char RightTrigger;
	};

	enum class DS_TRIGGERMODE {
		Free,
		ContinuousResistance,
		StaticResistance
	};
	
	struct DS_TRIGGER {
		uint8_t Resistance;
		uint8_t ResistanceStart;
		uint8_t Rumble;
		DS_TRIGGERMODE Mode;
	};

	/// <summary>
	/// A report for a state of the controller
	/// </summary>
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

	class DualSense {
	public:
		/// <summary>
		/// Creates a new DS4 instance based on HID path
		/// </summary>
		/// <param name="path"></param>
		/// <param name="controllerNumber">The number of the controller</param>
		DualSense(char* path, uint8_t controllerNumber);

		/// <summary>
		/// Returns the mode of the controller
		/// </summary>
		/// <returns>The mode</returns>
		[[nodiscard]] ControllerMode Mode() const;

		/// <summary>
		/// Returns the path of the device
		/// </summary>
		/// <returns>The path</returns>
		[[nodiscard]] char* Path() const;

		[[nodiscard]] PVIGEM_TARGET Target() const;

		/// <summary>
		/// Gets the MAC address of the device
		/// </summary>
		/// <returns>The mac as a string</returns>
		std::string MAC() const;

		/// <summary>
		/// Sets the color of the LED
		/// </summary>
		/// <param name="color">The color to use</param>
		void SetLEDColor(DS_LIGHTBARCOLOR color);

		/// <summary>
		/// Sets the trigger state of a given trigger
		/// </summary>
		/// <param name="trigger">The trigger data</param>
		/// <param name="triggerId">The Id of the trigger, 0 = left, right = 1</param>
		void SetTrigger(DS_TRIGGER trigger, uint8_t triggerId);

		/// <summary>
		/// Sets rumble for a motor
		/// </summary>
		/// <param name="rumble">The strength</param>
		/// <param name="type">The motor to use</param>
		void SetRumble(uint8_t rumble, DS_RUMBLE type);

		/// <summary>
		/// Sets the mute state
		/// </summary>
		/// <param name="muted">The state</param>
		void SetMute(bool muted);

		/// <summary>
		/// Sets the mode of the controller.
		/// </summary>
		/// <param name="mode">The mode to use</param>
		void SetMode(ControllerMode mode);

		/// <summary>
		/// Sets the target of this device
		/// </summary>
		/// <param name="target"></param>
		void SetTarget(PVIGEM_TARGET target);
		
		/// <summary>
		/// Sets the callback to trigger on new report data
		/// </summary>
		/// <param name="reportCallback">The callback to use</param>
		void SetReportCallback(std::function<void(DS_REPORT)> reportCallback);

		/// <summary>
		/// Sets the callback to trigger on mode change
		/// </summary>
		/// <param name="modeCallback">The callback to use</param>
		void SetModeCallback(std::function<void(ControllerMode)> modeCallback);

	private:
		// Device data
		char* _path;
		hid_device* _device;
		PVIGEM_TARGET _target;
		ControllerMode _mode;
		DS_LIGHTBARCOLOR _color;
		uint8_t _controllerNumber;
		bool _isMuted;
		DS_TRIGGER _triggerLeft;
		DS_TRIGGER _triggerRight;
		uint8_t _rumbleHeavy;
		uint8_t _rumbleLight;

		// Data 
		unsigned char _buffer[32];
		std::thread _thread;
		std::function<void(DS_REPORT)> _reportCallback;
		std::function<void(ControllerMode)> _modeCallback;


		/// <summary>
		/// Runs a thread routine that reads and writes data
		/// </summary>
		void Routine();

		/// <summary>
		/// Writes data to the DS. WIP
		/// </summary>
		void WriteReports();

		/// <summary>
		/// Reads data from the DS
		/// </summary>
		void ReadReports();
	};
}
