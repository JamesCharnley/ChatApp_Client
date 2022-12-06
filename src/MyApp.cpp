#include "MyApp.h"
#include "Ultralight/RefPtr.h"

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

Client* global_client = nullptr;
MyApp* global_MyApp = nullptr;

MyApp::MyApp() {

    global_MyApp = this;
  ///
  /// Create our main App instance.
  ///
  app_ = App::Create();

  ///
  /// Create a resizable window by passing by OR'ing our window flags with
  /// kWindowFlags_Resizable.
  ///
  window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Resizable);

  ///
  /// Create our HTML overlay-- we don't care about its initial size and
  /// position because it'll be calculated when we call OnResize() below.
  ///
  side_panel = Overlay::Create(window_, 1, 1, 0, 0);
  side_panel_view = HTML_View_Sidepanel(side_panel->view().get());
  room_panel = Overlay::Create(window_, 1, 1, 0, 0);
  login_panel = Overlay::Create(window_, 1, 1, 0, 0);
  dashboard_panel = Overlay::Create(window_, 1, 1, 0, 0);

  ///
  /// Force a call to OnResize to perform size/layout of our overlay.
  ///
  OnResize(window_.get(), window_->width(), window_->height());

  ///
  /// Load a page into our overlay's View
  ///
  side_panel->view()->LoadURL("file:///app.html");
  room_panel->view()->LoadURL("file:///app1.html");
  login_panel->view()->LoadURL("file:///Login.html");
  dashboard_panel->view()->LoadURL("file:///dashboard.html");

  ///
  /// Register our MyApp instance as an AppListener so we can handle the
  /// App's OnUpdate event below.
  ///
  app_->set_listener(this);

  ///
  /// Register our MyApp instance as a WindowListener so we can handle the
  /// Window's OnResize event below.
  ///
  window_->set_listener(this);

  ///
  /// Register our MyApp instance as a LoadListener so we can handle the
  /// View's OnFinishLoading and OnDOMReady events below.
  ///
  side_panel->view()->set_load_listener(this);
  room_panel->view()->set_load_listener(this);
  login_panel->view()->set_load_listener(this);
  dashboard_panel->view()->set_load_listener(this);


  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  side_panel->view()->set_view_listener(this);
  room_panel->view()->set_view_listener(this);
  login_panel->view()->set_view_listener(this);
  dashboard_panel->view()->set_view_listener(this);
  

  //callers.push_back(overlay_->view().get());
  callers.push_back(room_panel->view().get());

  //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "hello");



  USHORT LocalPort = 8000;
  USHORT ServerPort = 8080;
  char add[20];
  std::string str = "192.168.1.107";
  for (int i = 0; i < str.length(); i++)
  {
      add[i] = str[i];
  }
  int len = (int)str.length();
  add[len] = '\0';

  clientNetworking = new Client(add, ServerPort, LocalPort, this);
  global_client = clientNetworking;
   
  //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "end of constructor");
}

MyApp::~MyApp() {
    delete clientNetworking;
}

void MyApp::Run() {
  app_->Run();
}

void MyApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
  /// 
    //CheckQueue();
    if (b_UserLoggedIn)
    {
        b_UserLoggedIn = false;
        FString_Packet p = { ECommand::Authorized, username };
        UserLoggedIn(p);
    }
    CheckQueue();
}

void MyApp::OnClose(ultralight::Window* window) {
  app_->Quit();
}

void MyApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  side_panel->Resize(width, height);
  room_panel->Resize(width, height);
  login_panel->Resize(width, height);
  dashboard_panel->Resize(width, height);

  uint32_t left_pane_width_px = window_->ScreenToPixels(300);
  side_panel->Resize(left_pane_width_px, height);

  // Calculate the width of our right pane (window width - left width)
  int right_pane_width = (int)width - left_pane_width_px;

  // Clamp our right pane's width to a minimum of 1
  right_pane_width = right_pane_width > 1 ? right_pane_width : 1;

  room_panel->Resize((uint32_t)right_pane_width, height);
  login_panel->Resize((uint32_t)right_pane_width, height);
  dashboard_panel->Resize((uint32_t)right_pane_width, height);
  side_panel->MoveTo(0, 0);
  room_panel->MoveTo(left_pane_width_px, 0);
  login_panel->MoveTo(left_pane_width_px, 0);
  dashboard_panel->MoveTo(left_pane_width_px, 0);
}


void MyApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

// This callback will be bound to 'ChangeRoom()' on the HTML page.
JSValueRef ChangeRoom(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("ChangeRoom() called"));

    std::string* strs = new std::string[argumentCount];

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
    
    std::string packaged = "";
    for (int i = 0; i < argumentCount; i++)
    {
        packaged += strs[i];
        if (i < argumentCount - 1)
        {
            packaged += ";";
        }
    }
    global_MyApp->ChangeRoom_CPP(packaged);

    delete[] strs;

    return JSValueMakeNull(ctx);
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
    global_client->SendMessageToServer(packaged);

    delete[] strs;

    return JSValueMakeNull(ctx);
}

// This callback will be bound to 'OnButtonClick()' on the page.
JSValueRef OnButtonClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    const char* str =
        "document.getElementById('result').innerText = 'Ultralight rocks!'";
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("OnButtonClick"));
    // Create our string of JavaScript
    JSStringRef script = JSStringCreateWithUTF8CString(str);

    // Execute it with JSEvaluateScript, ignoring other parameters for now
    JSEvaluateScript(ctx, script, 0, 0, 0, 0);

    // Release our string (we only Release what we Create)
    JSStringRelease(script);

    if (argumentCount > 0)
    {
        //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "argument count > 1");
        JSType argType = JSValueGetType(ctx, arguments[0]);
        if (argType == JSType::kJSTypeString)
        {
            //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/defaut/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "argument type == string");
            JSStringRef msgArgumentJSRef = JSValueToStringCopy(ctx, arguments[0], NULL);
            size_t length = JSStringGetLength(msgArgumentJSRef) + 1;
            std::unique_ptr<char[]> stringBuffer = std::make_unique<char[]>(length);
            JSStringGetUTF8CString(msgArgumentJSRef, stringBuffer.get(), length);
            ultralight::String str(stringBuffer.get(), length);
            ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, str);
        }
        else if (argType == JSType::kJSTypeNull)
        {
            //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "argument type == null");
        }
        else if (argType == JSType::kJSTypeUndefined)
        {
            //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "argument type == undefined");
        }
        else
        {
            //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "argument type unknown");
        }
    }

    return JSValueMakeNull(ctx);
}

void MyApp::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {
  ///
  /// This is called when a frame's DOM has finished loading on the page.
  ///
  /// This is the best time to setup any JavaScript bindings.
  ///

    if (isAuthenticated == false)
    {
        if (caller == room_panel->view())
        {
            if (!room_panel->is_hidden())
            {
                room_panel->Hide();
            }
        }
        else if (caller == dashboard_panel->view())
        {
            if (!dashboard_panel->is_hidden())
            {
                dashboard_panel->Hide();
            }
        }
        else if(caller == login_panel->view())
        {
            if (login_panel->is_hidden())
            {
                login_panel->Show();
            }
        }
    }
    else
    {
        if (caller == dashboard_panel->view())
        {
            if (dashboard_panel->is_hidden())
            {
                dashboard_panel->Show();
            }
        }
        else if (caller == room_panel->view())
        {
            if (!room_panel->is_hidden())
            {
                room_panel->Hide();
            }
        }
        else if (caller == login_panel->view())
        {
            if (!login_panel->is_hidden())
            {
                login_panel->Hide();
            }
        }
    }

    if (caller == room_panel->view())
    {
        
        CheckQueue();

        // Acquire the JS execution context for the current page.
  //
  // This call will lock the execution context for the current
  // thread as long as the Ref<> is alive.

        ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
        // Get the underlying JSContextRef for use with the
        // JavaScriptCore C API.
        JSContextRef ctx = context->ctx();

        // Create a JavaScript String containing the name of our callback.
        JSStringRef name = JSStringCreateWithUTF8CString("OnButtonClick");

        // Create a garbage-collected JavaScript function that is bound to our
        // native C callback 'OnButtonClick()'.
        JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
            OnButtonClick);

        // Get the global JavaScript object (aka 'window')
        JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

        // Store our function in the page's global JavaScript object so that it
        // accessible from the page as 'OnButtonClick()'.
        JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

        // Release the JavaScript String we created earlier.
        JSStringRelease(name);


    }
    if (caller == login_panel->view())
    {
        ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
        // Get the underlying JSContextRef for use with the
        // JavaScriptCore C API.
        JSContextRef ctx = context->ctx();

        // Create a JavaScript String containing the name of our callback.
        JSStringRef name = JSStringCreateWithUTF8CString("CPPLogin");

        // Create a garbage-collected JavaScript function that is bound to our
        // native C callback 'OnButtonClick()'.
        JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
            CPPLogin);

        // Get the global JavaScript object (aka 'window')
        JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

        // Store our function in the page's global JavaScript object so that it
        // accessible from the page as 'OnButtonClick()'.
        JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

        // Release the JavaScript String we created earlier.
        JSStringRelease(name);
    }

    if (caller == side_panel->view())
    {
        side_panel_view.DOMLoaded = true;
        side_panel_view.Update();
    }

    if (caller == dashboard_panel->view())
    {
        ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
        // Get the underlying JSContextRef for use with the
        // JavaScriptCore C API.
        JSContextRef ctx = context->ctx();

        // Create a JavaScript String containing the name of our callback.
        JSStringRef name = JSStringCreateWithUTF8CString("ChangeRoom");

        // Create a garbage-collected JavaScript function that is bound to our
        // native C callback 'OnButtonClick()'.
        JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
            ChangeRoom);

        // Get the global JavaScript object (aka 'window')
        JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

        // Store our function in the page's global JavaScript object so that it
        // accessible from the page as 'OnButtonClick()'.
        JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

        // Release the JavaScript String we created earlier.
        JSStringRelease(name);
    }
   
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("DOMREADY END"));
}

void MyApp::OnChangeCursor(ultralight::View* caller,
                           Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller,
                          const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}

std::vector<std::string> queue = std::vector<std::string>();
void MyApp::UpdateElement(std::string _message)
{
    std::unique_lock<std::mutex>lock(Message_Queue_Mutex);
    queue.push_back(_message);
    
    
}

void MyApp::CheckQueue()
{
    std::lock_guard<std::mutex>lock(Message_Queue_Mutex);
    if (queue.size() > 0)
    {
        std::string i2s = std::to_string(queue.size());
        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "Queue size:");
        const char* ss = i2s.c_str();
        ultralight::String us(ss);
        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, us);
        for (int j = 0; j < queue.size(); j++)
        {
            std::vector<std::string>::iterator iter = queue.begin() + j;
            if (iter != queue.end())
            {
                std::string msgs = *iter;
                ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "Message in queue:");
                ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, msgs.c_str());
                for (int i = 0; i < callers.size(); i++)
                {
                    std::vector<ultralight::View*>::iterator it = callers.begin() + i;
                    if (it != callers.end())
                    {
                        ultralight::View* caller = *it;
                        if (caller != nullptr)
                        {
                            ultralight::String functionString = "";
                            ultralight::String funcStart = "AddMessage('";
                            ultralight::String funcParam(msgs.c_str());
                            ultralight::String funcEnd = "')";
                            
                            functionString += funcStart;
                            functionString += funcParam;
                            functionString += funcEnd;
                            ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("Function String:"));
                            ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, functionString);
                            caller->EvaluateScript(functionString);
                            caller->EvaluateScript("updateScroll()");
                        }
                    }
                }
            }
            
        }
        queue.clear();

    }
    //Message_Queue_cv.notify_one();
    
}

void MyApp::UserLoggedIn(FString_Packet _packet)
{
    // pass it to HTML_View_Sidepanel class
    side_panel_view.SetUsername(_packet._string);
    user.Username = _packet._string;
    isAuthenticated = true;

    if (!login_panel->is_hidden())
    {
        login_panel->Hide();
    }
    if (dashboard_panel->is_hidden())
    {
        dashboard_panel->Show();
    }
}

void MyApp::ChangeRoom_CPP(std::string _room)
{
    int com = (int)ECommand::GetRoom;
    std::string com_string = std::to_string(com);

    std::string packetString = com_string + ";" + _room;

    clientNetworking->SendMessageToServer(packetString);

    if (!dashboard_panel->is_hidden())
    {
        dashboard_panel->Hide();
    }
    if (room_panel->is_hidden())
    {
        room_panel->Show();
    }
}


//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("user logged in"));
//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, _packet._string);
//user.Username = _packet._string;
//
//char* chartest = new char[3];
//chartest[0] = 'h';
//chartest[1] = 'i';
//chartest[2] = '\0';
//std::string test(chartest);
////ultralight::String ulTest(test.c_str());