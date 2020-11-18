#pragma once

#include <map>
#include <string>
#include <memory>
#include <array>
#include <sigslot/signal.hpp>
#include <AppCore/AppCore.h>

#include "DualSense.hxx"

namespace BrokenBytes::DualSense4Windows::UI {
	constexpr uint32_t WINDOW_WIDTH = 800;
	constexpr uint32_t WINDOW_HEIGHT = 600;

	inline std::array<char*, 2> JS_FUNC_ARR = {
		"OnDualSenseDevicesUpdated",
		"Test"
	};
	
	// JS Functions
	enum class JS_FUNCTIONS {
		OnDualSenseDevicesUpdated,
		Test
	};

	inline char* JS_FUNC(JS_FUNCTIONS func, const char* data, size_t length) {
		auto f = JS_FUNC_ARR[static_cast<int>(func)];
		char* str = new char[255];
		memset(str, 0, 255);
		strcat_s(str, 255, f);
		strcat_s(str, 255, "(");
		strcat_s(str, 255, data);
		strcat_s(str, 255, ")\0");
		return str;
	};

	inline char* JS_FUNC(JS_FUNCTIONS func, std::string data) {
		return JS_FUNC(func, data.c_str(), data.size());
	};

	inline char* JS_FUNC(JS_FUNCTIONS func) {
		auto f = JS_FUNC_ARR[static_cast<int>(func)];
		char* str = new char[255];
		memset(str, 0, 255);
		auto len = strlen(f);
		strcat_s(str, 255, f);
		len = strlen(str);
		strcat_s(str, 255, "()\0");
		return str;
	};
	
	class App :
		public ultralight::AppListener,
		public ultralight::LoadListener,
		public ultralight::ViewListener,
		public ultralight::WindowListener
	{

    public:
		sigslot::signal<> AppStarted;
		
	    ~App();
		/// <summary>
		/// Creates or returns the singleton instance of App
		/// </summary>
		/// <returns>The singleton</returns>
		[[nodiscard]] static std::shared_ptr<App> Instance();
		void Run();
	    void OnUpdate() override;
	    void OnClose() override;
	    void OnResize(uint32_t width, uint32_t height) override;
	    void OnChangeTitle(ultralight::View* caller, const ultralight::String& title) override;
	    void OnChangeURL(ultralight::View* caller, const ultralight::String& url) override;
	    void OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) override;
	    void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;
	    void OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
		    ultralight::MessageLevel level, const ultralight::String& message, uint32_t line_number,
		    uint32_t column_number, const ultralight::String& source_id) override;
	    ultralight::RefPtr<ultralight::View> OnCreateChildView(ultralight::View* caller,
		    const ultralight::String& opener_url, const ultralight::String& target_url, bool is_popup,
		    const ultralight::IntRect& popup_rect) override;
	    void OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		    const ultralight::String& url) override;
	    void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		    const ultralight::String& url) override;
	    void OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		    const ultralight::String& url, const ultralight::String& description,
		    const ultralight::String& error_domain, int error_code) override;
	    void OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		    const ultralight::String& url) override;
	    void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
		    const ultralight::String& url) override;
	    void OnUpdateHistory(ultralight::View* caller) override;

		// Slots
		/// <summary>
		/// Called when the number of DualSense devices connected changes
		/// </summary>
		/// <param name="devices"></param>
		void DualSenseDevicesChanged(std::map<char*, DualSense*> devices);


	private:
		static inline std::weak_ptr<App> _appInstance;
		ultralight::RefPtr<ultralight::App> _app;
		ultralight::RefPtr<ultralight::Window> _window;
		ultralight::RefPtr<ultralight::Overlay> _overlay;

		App();
    }; 
}
