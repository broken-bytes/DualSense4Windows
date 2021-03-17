#include <memory>
#include "MainWindow.hxx"
#include "ColorPicker.hxx"
#include "Window.hxx"

#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>

namespace BrokenBytes::DualSense4Windows::UI {
	MainWindow::MainWindow() : QMainWindow(nullptr, {}) {
		_list = std::make_unique<DualSenseList>(this);
		_list->setFixedSize(1024, 256);
		setCentralWidget(_list.get());
		DevicesChanged.connect(&DualSenseList::Refresh, _list.get());
		setPalette({ Qt::gray, {35 , 35, 50} });
	}
	
	MainWindow::~MainWindow() {}
	void MainWindow::Run() {}
	void MainWindow::OnUpdate() {}
	void MainWindow::OnClose() {}
	void MainWindow::RegisterWindows() {}

	void MainWindow::DualSenseDevicesChanged(std::vector<char*> devices) {
		DevicesChanged(devices);
	}
}


