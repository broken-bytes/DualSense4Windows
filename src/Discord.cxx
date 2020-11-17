#include "Discord.hxx"

namespace BrokenBytes::DualSense4Windows::Discord {
	Wrapper::Wrapper() {
		discord::Core* core;
		discord::Core::Create(CLIENT_ID, DiscordCreateFlags_Default, &core);
		_discord = std::unique_ptr<discord::Core>(core);
	}

	std::shared_ptr<Wrapper> Wrapper::Create() {
		if(auto ptr = Wrapper::_application.lock()) {
			return ptr;
		}
		return std::shared_ptr<Wrapper>();
	}

	bool Wrapper::Mute() const {
		bool* muted = nullptr;
		_discord->VoiceManager().IsSelfMute(muted);
		bool m = *muted;
		delete muted;
		return m;
	}
	bool Wrapper::Deaf() const {
		bool* deafened = nullptr;
		_discord->VoiceManager().IsSelfMute(deafened);
		bool d = *deafened;
		delete deafened;
		return d;
	}
}
