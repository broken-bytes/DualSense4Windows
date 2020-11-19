#include "Core.hxx"
#include <CommCtrl.h>
#include <cstdint>

#include "MainWindow.hxx"

namespace BrokenBytes::DualSense4Windows::UI {

	constexpr wchar_t MAIN_WINDOW[] = L"MAIN_WINDOW";
	constexpr uint16_t WIDTH = 800;
	constexpr uint16_t HEIGHT = 800;

	/// Ignore wchar_t* literal warning for LPWSTR
	constexpr std::array<LPWSTR, 3> TABS = {
		L"Controllers",  // NOLINT(clang-diagnostic-writable-strings)
		L"Profiles", // NOLINT(clang-diagnostic-writable-strings)
		L"Settings" // NOLINT(clang-diagnostic-writable-strings)
	};

	/// Ignore wchar_t* literal warning for LPWSTR
	constexpr std::array<LPWSTR, 5> COLUMNS = {
		L"ID",  // NOLINT(clang-diagnostic-writable-strings)
		L"Connection", // NOLINT(clang-diagnostic-writable-strings)
		L"Mode", // NOLINT(clang-diagnostic-writable-strings)
		L"Color", // NOLINT(clang-diagnostic-writable-strings)
		L"Profile" // NOLINT(clang-diagnostic-writable-strings)
	};

	
	
	MainWindow::MainWindow(
		LPCWSTR title,
		uint16_t width,
		uint16_t height
	) : Window(title, width, height) {
		_tabView = CreateTabControl();
		_listView = CreateListViewControl();
		SetFont(_tabView, FONT_TAB);
	}
	
	void MainWindow::Show() {
		Window::Show();
		auto color = new ColorPicker(Color { 120, 120, 120});
		color->ColorChanged.connect([this](Color c) {
			ColorChanged(0, c);
		});
	}
	
	void MainWindow::Hide() {
		Window::Hide();
	}

	void MainWindow::DualSenseDevicesChanged(std::vector<char*> devices) {
		// Remove removed devices

		if(_devices.empty()) {
			_devices = devices;
			for(int x = 0; x < devices.size(); x++) {
				AddControls(devices[x], x);
			}
			return;
		}
		
		for (int x = 0; x < _devices.size(); x++) {
			const auto found = std::find(
				_devices.begin(),
				_devices.end(),
				devices[x]
			) != _devices.end();
			if (!found) {
				_devices.erase(_devices.begin() + x);
				RemoveControls(devices[x]);
			}
		}
		
		// Add new devices
		for (int x = 0; x < _devices.size(); x++) {
			const auto found = std::find(
				_devices.begin(),
				_devices.end(),
				devices[x]
			) != _devices.end();
			if (!found) {
				_devices.erase(_devices.begin() + x);
				AddControls(devices[x], x);
			}
		}

		UpdateWindow(Handle());
	}

	LRESULT CALLBACK MainWindow::ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		if(uMsg == WM_DESTROY) {
			PostQuitMessage(0);
		}

		if(uMsg == WM_DEVICECHANGE) {
			DevicesChanged();
		}

		if (uMsg == WM_PAINT) {
			
		}
		
		return DefWindowProc(Handle(), uMsg, wParam, lParam);
	}

	HWND MainWindow::CreateTabControl() {
		RECT rcClient;
		INITCOMMONCONTROLSEX icex;
		TCITEM tie;

		// Initialize common controls.
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&icex);
		GetClientRect(Handle(), &rcClient);
		auto tabView = CreateWindow(
			WC_TABCONTROL,
			L"",
			WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
			0,
			0,
			rcClient.right,
			rcClient.bottom,
			Handle(),
			NULL,
			GetModuleHandle(nullptr),
			NULL
		);

		// Add tabs for each day of the week. 
		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;

		for(int x = 0; x < TABS.size(); x++) {
			tie.pszText = TABS[x];
			if (TabCtrl_InsertItem(tabView, x, &tie) == -1) {
				DestroyWindow(tabView);
				return nullptr;
			}
		}
		
		return tabView;
	}

	HWND MainWindow::CreateListViewControl() {

		RECT r;
		GetWindowRect(_tabView, &r);

		INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
		icex.dwICC = ICC_LISTVIEW_CLASSES;
		InitCommonControlsEx(&icex);
		auto listView = CreateWindow(WC_LISTVIEW,
			L"ListView",
			WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
			0, r.top - 10,
			r.right - r.left - 4,
			r.bottom,
			_tabView,
			(HMENU)0,
			GetModuleHandle(nullptr),
			NULL);

		ShowWindow(listView, SW_SHOW);

		WCHAR szText[256];     // Temporary buffer.
		LVCOLUMN lvc;

		// Initialize the LVCOLUMN structure.
		// The mask specifies that the format, width, text,
		// and subitem members of the structure are valid.
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		// Add the columns.
		for (int x = 0; x < COLUMNS.size(); x++)
		{
			lvc.iSubItem = x;
			lvc.pszText = szText;
			lvc.cx = 100;               // Width of column in pixels.

			if (x < 2)
				lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
			else
				lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

			// Load the names of the column headings from the string resources.

			lvc.pszText = COLUMNS[x];

			// Insert the columns into the list view.
			if (ListView_InsertColumn(listView, x, &lvc) == -1) {
				return nullptr;
			}
		}

		return listView;
	}

	void MainWindow::AddControls(char* id, int index) {
		_info.emplace_back(DualSenseInfo{
			id,
			L"DualSense",
			ConnectionType::USB,
			Mode::XBox,
			Color { 255, 0, 255}
		});
		LVITEM lvI;
		// Initialize LVITEM members that are common to all items.
		lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvI.stateMask = 0;
		lvI.iSubItem = 0;
		lvI.state = 0;

		// Initialize LVITEM members that are different for each item.
		for (int x = 0; x < _info.size(); x++)
		{
			lvI.iItem = x;
			lvI.iImage = x;

			LVITEM _id;
			_id.iItem = x;
			_id.pszText = L"DualSense";
			_id.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			_id.stateMask = 0;
			_id.iSubItem = 0;
			_id.state = 0;

			LVITEM connection;
			connection.iItem = x;
			connection.pszText = (_info[x].Connection == ConnectionType::Bluetooth) ? L"Bluetooth" : L"USB";
			connection.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			connection.stateMask = 0;
			connection.iSubItem = 1;
			connection.state = 0;

			LVITEM mode;
			mode.iItem = x;
			LPWSTR str = L"";
			switch (_info[x].Mode) {
			case Mode::DS4:
				str = L"DualShock4";
				break;
			case Mode::XBox:
				str = L"XBox360";
				break;
			default:
				str = L"DualSense";
			}
			mode.pszText = str;
			mode.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			mode.stateMask = 0;
			mode.iSubItem = 2;
			mode.state = 0;
			
			
			// Insert items into the list.
			if (ListView_InsertItem(_listView, &lvI) == -1) {
				return;
			}
			SendMessage(_listView, LVM_SETITEM, x, reinterpret_cast<LPARAM>(&_id));
			SendMessage(
				_listView,
				LVM_SETITEM,
				x,
				reinterpret_cast<LPARAM>(&connection)
			);
			SendMessage(_listView, LVM_SETITEM, x, reinterpret_cast<LPARAM>(&mode));
		}

		return;
	}
	
	void MainWindow::RemoveControls(char* id) {}
}
