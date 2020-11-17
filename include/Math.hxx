#pragma once

#include <cstdint>
#include <array>
#include <ios>
#include <iostream>

#include <crc/CRC.h>

namespace BrokenBytes::DualSense4Windows::Math {
	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector2 {
		T x;
		T y;
	};

	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector3 {
		T x;
		T y;
		T z;
	};

	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector4 {
		T x;
		T y;
		T z;
		T w;
	};

	/// <summary>
	/// Converts a value from uint range to float range from (-1) - 1
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline float ConvertToSignedFloat(uint8_t value) {
		auto f = static_cast<float>(value);
		f /= 255;
		return static_cast<float>((f * 2) - 1.0f);
	}

	/// <summary>
	/// Converts a value from uint range to float range from 0 - 1
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline float ConvertToUnsignedFloat(uint8_t value) {
		auto f = static_cast<float>(value);
		f /= 255;
		return static_cast<float>(f);
	}


	/// <summary>
	/// Gets the CRC-32 checksum for a range of bytes
	/// </summary>
	/// <param name="bytes"></param>
	/// <returns>The CRC Checksum split into 4 bytes</returns>
	inline std::array<uint8_t, 4> GetCRCFromBytes(unsigned char* bytes, size_t length) {
		std::uint32_t crc = CRC::Calculate(bytes, length, CRC::CRC_32());
		auto arr = std::array<uint8_t, 4> {
			static_cast<unsigned char>(crc),
				static_cast<unsigned char>(crc >> 8),
				static_cast<unsigned char>(crc >> 16),
				static_cast<unsigned char>(crc >> 24),
		};

		return arr;
	}
}
