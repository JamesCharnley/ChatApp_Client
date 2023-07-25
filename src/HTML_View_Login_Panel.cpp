#include "HTML_View_Login_Panel.h"
#include "MyApp.h"
#include "Client.h"

MyApp* login_global_myApp = nullptr;
Client* login_global_client = nullptr;

HTML_View_Login_Panel::HTML_View_Login_Panel()
{
}

HTML_View_Login_Panel::HTML_View_Login_Panel(View* _overlay, RefPtr<Overlay> _ref, MyApp* _myApp, Client* _client) : HTML_View::HTML_View(_overlay, _ref, _myApp)
{
	login_global_myApp = _myApp;
    login_global_client = _client;
}

void HTML_View_Login_Panel::Resize(ultralight::Window* window, uint32_t width, uint32_t height)
{
	HTML_View::Resize(window, width, height);

	uint32_t left_pane_width_px = window->ScreenToPixels(300);
	//side_panel->Resize(left_pane_width_px, height);

	// Calculate the width of our right pane (window width - left width)
	int right_pane_width = (int)width - left_pane_width_px;

	// Clamp our right pane's width to a minimum of 1
	right_pane_width = right_pane_width > 1 ? right_pane_width : 1;

	ref->Resize((uint32_t)right_pane_width, height);

	ref->MoveTo(left_pane_width_px, 0);
}

// This callback will be bound to 'CPPLogin()' on the page.
JSValueRef CPPLogin(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CPPLogin() called"));

    std::string* strs = new std::string[3];

    for (int i = 0; i < argumentCount; i++)
    {
        JSType argType = JSValueGetType(ctx, arguments[i]);
        if (argType == JSType::kJSTypeString)
        {
            JSStringRef msgArgumentJSRef = JSValueToStringCopy(ctx, arguments[i], NULL);
            size_t length = JSStringGetLength(msgArgumentJSRef) + 1;
            std::unique_ptr<char[]> stringBuffer = std::make_unique<char[]>(length);
            JSStringGetUTF8CString(msgArgumentJSRef, stringBuffer.get(), length);
            //ultralight::String str(stringBuffer.get(), length);
            std::string str(stringBuffer.get());
            strs[i] = str;

        }
    }
    strs[0] = "1";
    std::string packaged = strs[0] + ";" + strs[1] + ";" + strs[2];
    login_global_client->SendMessageToServer(packaged);

    delete[] strs;

    //FString_Packet p = { ECommand::Authorized, "james" };
    //global_MyApp->UserLoggedIn(p);

    return JSValueMakeNull(ctx);
}

// This callback will be bound to 'CPPLogin()' on the page.
JSValueRef CPPSignup(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CPPSignup() called"));

    std::string* strs = new std::string[3];

    for (int i = 0; i < argumentCount; i++)
    {
        JSType argType = JSValueGetType(ctx, arguments[i]);
        if (argType == JSType::kJSTypeString)
        {
            JSStringRef msgArgumentJSRef = JSValueToStringCopy(ctx, arguments[i], NULL);
            size_t length = JSStringGetLength(msgArgumentJSRef) + 1;
            std::unique_ptr<char[]> stringBuffer = std::make_unique<char[]>(length);
            JSStringGetUTF8CString(msgArgumentJSRef, stringBuffer.get(), length);
            //ultralight::String str(stringBuffer.get(), length);
            std::string str(stringBuffer.get());
            strs[i] = str;

        }
    }
    strs[0] = "2";
    std::string packaged = strs[0] + ";" + strs[1] + ";" + strs[2];
    login_global_client->SendMessageToServer(packaged);

    delete[] strs;

    return JSValueMakeNull(ctx);
}

void HTML_View_Login_Panel::BindJavaScriptFunctions()
{
    ultralight::RefPtr<ultralight::JSContext> context = ref.get()->view()->LockJSContext();
    // Get the underlying JSContextRef for use with the
    // JavaScriptCore C API.
    JSContextRef ctx = context->ctx();

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name = JSStringCreateWithUTF8CString("CPPLogin");

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name_signup = JSStringCreateWithUTF8CString("CPPSignup");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
        CPPLogin);

    JSObjectRef func_signup = JSObjectMakeFunctionWithCallback(ctx, name,
        CPPSignup);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);


    JSObjectSetProperty(ctx, globalObj, name_signup, func_signup, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name);

    JSStringRelease(name_signup);
}
