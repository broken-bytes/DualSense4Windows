#pragma once
#include <memory>

#include <discord/discord.h>

namespace BrokenBytes::DualSense4Windows::Discord {
	inline constexpr uint8_t CLIENT_ID = 0;
	
	struct Application {
		struct IDiscordCore* core;
		struct IDiscordUsers* users;
	};

	class Wrapper {
	public:
		/// <summary>
		/// Creates or returns the discord singleton instance
		/// </summary>
		/// <returns>The instance</returns>
		[[nodiscard]] static std::shared_ptr<Wrapper> Create();

		/// <summary>
		/// Checks if the locally running client is muted
		/// </summary>
		/// <returns>Mute state</returns>
		[[nodiscard]] bool Mute() const;

		/// <summary>
		/// Checks if the locally running client is deafened
		/// </summary>
		/// <returns>Deaf state</returns>
		[[nodiscard]] bool Deaf() const;
	private:
		static inline std::weak_ptr<Wrapper> _application;
		std::unique_ptr<discord::Core> _discord;

		Wrapper();
	};
}
