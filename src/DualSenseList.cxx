#include "DualSenseList.hxx"

namespace BrokenBytes::DualSense4Windows::UI {

	DualSenseList::DualSenseList(QWidget* parent) : QWidget(parent) {
		_layout = std::make_unique<QVBoxLayout>(this);
	}

	void DualSenseList::Refresh(std::vector<char*>& list) {
		for (auto& item : _entries) {
			_layout->removeWidget(item.get());
		}
		_entries.clear();
		_entries.shrink_to_fit();
		for(auto& item: list) {
			auto ptr = 
				_entries.emplace_back(std::make_unique<DualSenseEntryWidget>(item, false, 0)).get();
			_layout->addWidget(ptr);
		}
	}
}
