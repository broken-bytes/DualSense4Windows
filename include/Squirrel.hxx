#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace BrokenBytes::Squirrel {
	struct Nut {
		std::string module;
		std::string event;
		std::string message;
	};
	
	class Squirrel {
	public:
		/// <summary>
		/// Creates the singleton instance of the Squirrel logger
		/// </summary>
		/// <returns>The instance</returns>
		static std::shared_ptr<Squirrel> Instance();

		/// <summary>
		/// Plants a new Nut(A log)
		/// </summary>
		/// <param name="module">The Module</param>
		/// <param name="event">The Event</param>
		/// <param name="message">The Message</param>
		void Plant(
			std::string module,
			std::string event,
			std::string message
		);

		/// <summary>
		/// Picks the last Nut. This reads the newest log but does not clear it
		/// </summary>
		/// <returns>A Nut</returns>
		[[nodiscard]] std::optional<Nut> Pick() const;
		
		/// <summary>
		/// Consumes the last Nut. This reads the newest log and removes it
		/// </summary>
		/// <returns>A Nut</returns>
		[[nodiscard]] std::optional<Nut> Consume();
		
	private:
		Squirrel();
		static inline std::weak_ptr<Squirrel> _instance;
		std::vector<Nut> _nuts;
	};

	inline void Squirrel::Plant(
		std::string module,
		std::string event,
		std::string message
	) {
		_nuts.emplace_back(Nut{ module, event, message });
	}

	inline Squirrel::Squirrel() {
		_nuts = std::vector<Nut>();
	}

	inline std::shared_ptr<Squirrel> Squirrel::Instance() {
		if(auto ptr = Squirrel::_instance.lock()) {
			return ptr;
		}
		return std::make_shared<Squirrel>(Squirrel());
	}

	inline std::optional<Nut> Squirrel::Pick() const {
		if(_nuts.empty()) {
			return std::nullopt;
		}
		return _nuts.front();
	}
	
	inline std::optional<Nut> Squirrel::Consume() {
		if(_nuts.empty()) {
			return std::nullopt;
		}
		auto nut = _nuts.front();
		_nuts.erase(_nuts.begin());
		return nut;
	}
}
