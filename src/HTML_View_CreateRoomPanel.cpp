#include "HTML_View_CreateRoomPanel.h"
#include "MyApp.h"
#include "Client.h"

MyApp* create_room_global_myApp = nullptr;
Client* create_room_global_client = nullptr;
HTML_View_CreateRoomPanel::HTML_View_CreateRoomPanel()
{
}

HTML_View_CreateRoomPanel::HTML_View_CreateRoomPanel(View* _overlay, RefPtr<Overlay> _ref, MyApp* _myApp, Client* _client)
{
}

void HTML_View_CreateRoomPanel::Resize(ultralight::Window* window, uint32_t width, uint32_t height)
{
	HTML_View::Resize(window, width, height);

	uint32_t left_pane_width_px = window->ScreenToPixels(300);

	// Calculate the width of our right pane (window width - left width)
	int right_pane_width = (int)width - left_pane_width_px;

	// Clamp our right pane's width to a minimum of 1
	right_pane_width = right_pane_width > 1 ? right_pane_width : 1;

	ref->Resize((uint32_t)right_pane_width, height);

	ref->MoveTo(left_pane_width_px, 0);
}

JSValueRef cpp_create_room(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    std::string room_name = "";

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
            room_name = str;

        }
    }

    create_room_global_myApp->create_room_cpp(room_name);

    return JSValueMakeNull(ctx);
}

void HTML_View_CreateRoomPanel::BindJavaScriptFunctions()
{
    ultralight::RefPtr<ultralight::JSContext> context = ref.get()->view()->LockJSContext();
    // Get the underlying JSContextRef for use with the
    // JavaScriptCore C API.
    JSContextRef ctx = context->ctx();

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name = JSStringCreateWithUTF8CString("cpp_create_room");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
        cpp_create_room);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name);

    ref->Hide();
}
