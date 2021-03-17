#include "DualSenseListEntry.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	DualSenseListEntry::DualSenseListEntry() {
		_layout = std::make_unique<QHBoxLayout>(this);
		_icon = std::make_unique<QPushButton>(this);
		_text = std::make_unique<QLabel>(this);
		_state = std::make_unique<QPushButton>(this);
		_layout->addWidget(_icon.get());
		_layout->addWidget(_text.get());
		_layout->addWidget(_state.get());
	}

	DualSenseListEntry::DualSenseListEntry(std::string name, bool state, float battery) : DualSenseListEntry() {
		_text->setText(name.c_str());
	}

}
