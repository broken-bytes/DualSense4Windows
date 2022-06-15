#include <map>
#include <memory>

#include <QtWidgets/QApplication>
#include <qtimer.h>
#include <QtWidgets/QPushButton>

#include "DualSense4Windows.hxx"

#include "Interface.hxx"
#include "MainWindow.hxx"
#include "Utils.hxx"



using namespace BrokenBytes::DualSense4Windows;

std::unique_ptr<QApplication> App;
std::unique_ptr<IO::Interface> Interface;
std::unique_ptr <::UI::MainWindow> Window;

int main(int argc, char** argv);



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	auto name = new char[] {"DualSense 4 Windows"};
	auto args = new char* [] {name};
	main(1, args);
}


int main(int argc, char** argv) {
	int args = 0;
	std::cout << *argv << std::endl;
	App = std::make_unique<QApplication>(argc, argv);
	auto timer = new QTimer();
	timer->setInterval(500);
	Interface = std::make_unique<IO::Interface>();
	QObject::connect(
		timer, &QTimer::timeout,
		Interface.get(), &IO::Interface::UpdateDualSenseDevices
	);
	timer->start();

	Window = std::make_unique<UI::MainWindow>();
	QObject::connect(
		Interface.get(), &IO::Interface::DevicesChanged,
		Window.get(), &UI::MainWindow::DualSenseDevicesChanged
	);
	Interface->Init();
	Window->setFixedWidth(1280);
	Window->setFixedHeight(768);
	Window->show();
	return App->exec();

}
