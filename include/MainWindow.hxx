#pragma once

#include <map>
#include <string>
#include <memory>
#include <array>
#include <sigslot/signal.hpp>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>

#include "ColorPicker.hxx"
#include "Core.hxx"

#include "DualSense.hxx"
#include "DualSenseList.hxx"
#include "Window.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	constexpr wchar_t WINDOW_NAME[] = L"DualSense4Windows";
	constexpr uint32_t WINDOW_WIDTH = 800;
	constexpr uint32_t WINDOW_HEIGHT = 600;

	class MainWindow : public QMainWindow {
		Q_OBJECT
	public:
		/// /// <summary>
		/// Creates an MainWindow instance
		/// </summary>
		MainWindow();
		~MainWindow();

		/// <summary>
		/// Runs the MainWindow loop
		/// </summary>
		void Run();

		/// <summary>
		/// Called on Update
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// Called on close of the MainWindow
		/// </summary>
		void OnClose();

	public slots:
		/// <summary>
		/// Called when the number of DualSense devices connected changes
		/// </summary>
		/// <param name="devices"></param>
		void DualSenseDevicesChanged(std::vector<char*> devices);

	signals:
		/// <summary>
		/// Called when the MainWindow starts
		/// </summary>
		void MainWindowStarted();

		/// <summary>
		/// Called when the MainWindow receives a device change event
		/// </summary>
		void DevicesChanged(std::vector<char*>);

		/// <summary>
		/// Called when the MainWindow triggers a color picker change
		/// </summary>
		void ColorChanged(uint8_t, Color);
	private:
		std::shared_ptr<UI::Window> _mainWindow;
		std::shared_ptr<UI::Window> _window;
		std::unique_ptr<QGridLayout> _layout;
		std::unique_ptr<DualSenseList> _list;

		void RegisterWindows();
	};
}
