#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

namespace BrokenBytes::DualSense4Windows::UI {
	class DualSenseListEntry : public QWidget {
	public:
		DualSenseListEntry();
		DualSenseListEntry(std::string name, bool state, float battery);

	private:
		std::unique_ptr<QHBoxLayout> _layout;
		std::unique_ptr<QPushButton> _icon;
		std::unique_ptr<QLabel> _text;
		std::unique_ptr<QPushButton> _state;
	};
}
