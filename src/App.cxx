#include <memory>

#include "App.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	App::App() {
		_app = ultralight::App::Create();
		_window = ultralight::Window::Create(
			_app->main_monitor(),
			WINDOW_WIDTH, WINDOW_HEIGHT,
			false,
			ultralight::kWindowFlags_Titled | ultralight::kWindowFlags_Resizable
		);
		_app->set_window(*_window);
		_overlay = ultralight::Overlay::Create(
			*_window,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0,
			0
		);

		this->OnResize(_window->width(), _window->height());
		_overlay->view()->LoadURL("file://assets/html/app.html");
		_app->set_listener(this);
		_window->set_listener(this);
		_overlay->view()->set_load_listener(this);
		_overlay->view()->set_view_listener(this);
	}
	
	App::~App() {
		
	}

	std::shared_ptr<App> App::Create() {
		if (auto ptr = App::_appInstance.lock()) {
			return ptr;
		}
		return std::shared_ptr<App>(new App());
	}


	void App::Run() {
		_app->Run();
	}
	
	void App::OnUpdate() {}
	void App::OnClose() {}
	void App::OnResize(uint32_t width, uint32_t height) {
		_overlay->Resize(width, height);
	}
	void App::OnChangeTitle(ultralight::View* caller, const ultralight::String& title) {
		_window->SetTitle(title.utf8().data());
	}
	void App::OnChangeURL(ultralight::View* caller, const ultralight::String& url) {}
	void App::OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) {}
	void App::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) {
		_window->SetCursor(cursor);
	}

	void App::OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
		ultralight::MessageLevel level, const ultralight::String& message, uint32_t line_number, uint32_t column_number,
		const ultralight::String& source_id) {}

	ultralight::RefPtr<ultralight::View> App::OnCreateChildView(ultralight::View* caller,
		const ultralight::String& opener_url, const ultralight::String& target_url, bool is_popup,
		const ultralight::IntRect& popup_rect) {
		return nullptr;
	}

	void App::OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		const ultralight::String& url) {}

	void App::OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		const ultralight::String& url) {}

	void App::OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		const ultralight::String& url, const ultralight::String& description, const ultralight::String& error_domain,
		int error_code) {}

	void App::OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		const ultralight::String& url) {}

	void App::OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		const ultralight::String& url) {}

	void App::OnUpdateHistory(ultralight::View* caller) {}
}


