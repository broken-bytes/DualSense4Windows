#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

namespace BrokenBytes::DualSense4Windows::UI {
	class DualSenseListHeader : public QWidget {
	public:
		DualSenseListHeader(QWidget* parent);

	private:
		std::unique_ptr<QHBoxLayout> _layout;
		std::unique_ptr<QLabel> _id;
		std::unique_ptr<QLabel> _state;
		std::unique_ptr<QLabel> _battery;
		std::unique_ptr<QLabel> _profile;
	};
}
