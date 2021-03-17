#pragma once

#include <vector>
#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>


#include "Core.hxx"
#include "DualSenseEntryWidget.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	class DualSenseList : public QWidget {
	public:
		DualSenseList(QWidget* parent);
		SLOT Refresh(std::vector<char*>& list);

	private:
		std::unique_ptr<QVBoxLayout> _layout;
		std::vector<std::unique_ptr<DualSenseEntryWidget>> _entries;
	};
}
