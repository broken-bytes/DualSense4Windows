#pragma once

#include <fstream>
#include <mutex>

namespace BrokenBytes::DualSense4Windows::Logger {
	static inline std::fstream LOG;
	static inline std::mutex m;
	inline void Log(std::string msg) {
		const std::lock_guard<std::mutex> lock(m);
		LOG.open("./log.txt",std::fstream::out | std::fstream::app);
		LOG << msg << "\n";
		LOG.close();
	}
}
