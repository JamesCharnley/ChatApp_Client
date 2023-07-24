#include "HTML_View_Sidepanel.h"
#include "MyApp.h"

MyApp* sidePanel_global_MyApp = nullptr;
HTML_View_Sidepanel::HTML_View_Sidepanel()
{
}

HTML_View_Sidepanel::HTML_View_Sidepanel(View* _overlay, RefPtr<Overlay> _ref, MyApp* _myApp) : HTML_View::HTML_View(_overlay, _ref, _myApp)
{
	username = "";
    sidePanel_global_MyApp = _myApp;
}

void HTML_View_Sidepanel::Update()
{
	UpdateUsername(username);
}



void HTML_View_Sidepanel::SetUsername(std::string _username)
{
	ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "SetUsername");
	username = _username;

	if (DOMLoaded == true)
	{
		Update();
	}
}

void HTML_View_Sidepanel::Resize(ultralight::Window* window, uint32_t width, uint32_t height)
{
	HTML_View::Resize(window, width, height);

	uint32_t left_pane_width_px = window->ScreenToPixels(300);
	ref->Resize(left_pane_width_px, height);
	ref->MoveTo(0, 0);
}

JSValueRef CPPOpenDashboard(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CPPOpenDashboard() called"));

    sidePanel_global_MyApp->clear_current_room();
    sidePanel_global_MyApp->open_dashboard();

    return JSValueMakeNull(ctx);
}
JSValueRef cpp_open_create_room_panel(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("open_create_room_panel() called"));

    sidePanel_global_MyApp->open_create_room_panel();
    return JSValueMakeNull(ctx);
}
void HTML_View_Sidepanel::BindJavaScriptFunctions()
{
    ultralight::RefPtr<ultralight::JSContext> context = ref.get()->view()->LockJSContext();
    // Get the underlying JSContextRef for use with the
    // JavaScriptCore C API.
    JSContextRef ctx = context->ctx();

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name = JSStringCreateWithUTF8CString("open_dashboard");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
        CPPOpenDashboard);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name);

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name_create_room = JSStringCreateWithUTF8CString("open_create_room_panel");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef func_open_create_room = JSObjectMakeFunctionWithCallback(ctx, name_create_room,
        cpp_open_create_room_panel);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, globalObj, name_create_room, func_open_create_room, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name_create_room);
}

//if (DOMLoaded == true)
//{
//	Update();
//}

void HTML_View_Sidepanel::UpdateUsername(std::string _username)
{
	String ul_string (_username.c_str());
	String log = "UpdateUsername(): " + String(_username.c_str());

	ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, log);
	
	String funcString = "";
	String funcStart = "SetUsername('";
	String funcParam = ul_string;
	String funcEnd = "')";

	funcString = funcStart + funcParam + funcEnd;
	
	overlay->EvaluateScript(funcString);
	
}

//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("b4 evaluate script"));
//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, funcString);
//
//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("after evaluate script"));