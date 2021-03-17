#include "DualSenseListHeader.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	DualSenseListHeader::DualSenseListHeader(QWidget* parent) : QWidget(parent) {
		_layout = std::make_unique<QHBoxLayout>(this);
		_id = std::make_unique<QLabel>(this);
		_state = std::make_unique<QLabel>(this);
		_battery = std::make_unique<QLabel>(this);
		_profile = std::make_unique<QLabel>(this);

		_id->setText("ID");
		_state->setText("State");
		_battery->setText("Battery");
		_profile->setText("Profile");
		
		_layout->addWidget(_id.get());
		_layout->addWidget(_state.get());
		_layout->addWidget(_battery.get());
		_layout->addWidget(_profile.get());
	}
}
