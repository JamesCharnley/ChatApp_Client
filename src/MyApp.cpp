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
  side_panel_view = HTML_View_Sidepanel(side_panel->view().get(), side_panel, this);
  htmlViews.push_back(&side_panel_view);

  room_panel = Overlay::Create(window_, 1, 1, 0, 0);
  login_panel = Overlay::Create(window_, 1, 1, 0, 0);
  dashboard_panel = Overlay::Create(window_, 1, 1, 0, 0);
  dashboard_panel_view = HTML_View_Dashboard(dashboard_panel->view().get(), dashboard_panel, this);
  htmlViews.push_back(&dashboard_panel_view);
  create_room_panel = Overlay::Create(window_, 1, 1, 0, 0);

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
  create_room_panel->view()->LoadURL("file:///CreateRoom.html");

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
  create_room_panel->view()->set_load_listener(this);


  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  side_panel->view()->set_view_listener(this);
  room_panel->view()->set_view_listener(this);
  login_panel->view()->set_view_listener(this);
  dashboard_panel->view()->set_view_listener(this);
  create_room_panel->view()->set_view_listener(this);

  //callers.push_back(overlay_->view().get());
  callers.push_back(room_panel->view().get());

  //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, "hello");



  USHORT LocalPort = 8000;
  USHORT ServerPort = 8080;
  char add[20];
  std::string str = "192.168.1.25";
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

    if (b_addroom)
    {
        FPost_Room_Packet p;
        AddRoom(p);
    }
    if (b_UserLoggedIn)
    {
        b_UserLoggedIn = false;
        FString_Packet p = { ECommand::Authorized, username };
        UserLoggedIn(p);
    }
    if (b_new_message)
    {
        CheckQueue();
    }
    
    clientNetworking->Tick();

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
  //side_panel->Resize(width, height);
  room_panel->Resize(width, height);
  login_panel->Resize(width, height);
  //ashboard_panel->Resize(width, height);
  create_room_panel->Resize(width, height);

  for (std::vector<HTML_View*>::iterator it = htmlViews.begin(); it != htmlViews.end(); it++)
  {
      HTML_View* view = *it;
      if (view != nullptr)
      {
          view->Resize(window, width, height);
      }
  }

  uint32_t left_pane_width_px = window_->ScreenToPixels(300);
  //side_panel->Resize(left_pane_width_px, height);

  // Calculate the width of our right pane (window width - left width)
  int right_pane_width = (int)width - left_pane_width_px;

  // Clamp our right pane's width to a minimum of 1
  right_pane_width = right_pane_width > 1 ? right_pane_width : 1;

  room_panel->Resize((uint32_t)right_pane_width, height);
  login_panel->Resize((uint32_t)right_pane_width, height);
  //dashboard_panel->Resize((uint32_t)right_pane_width, height);
  create_room_panel->Resize((uint32_t)right_pane_width, height);

  //side_panel->MoveTo(0, 0);
  room_panel->MoveTo(left_pane_width_px, 0);
  login_panel->MoveTo(left_pane_width_px, 0);
  //dashboard_panel->MoveTo(left_pane_width_px, 0);
  create_room_panel->MoveTo(left_pane_width_px, 0);
}


void MyApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

//// This callback will be bound to 'ChangeRoom()' on the HTML page.
//JSValueRef ChangeRoom(JSContextRef ctx, JSObjectRef function,
//    JSObjectRef thisObject, size_t argumentCount,
//    const JSValueRef arguments[], JSValueRef* exception) {
//
//    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("ChangeRoom() called"));
//
//    std::string* strs = new std::string[argumentCount];
//
//    for (int i = 0; i < argumentCount; i++)
//    {
//        JSType argType = JSValueGetType(ctx, arguments[i]);
//        if (argType == JSType::kJSTypeString)
//        {
//            JSStringRef msgArgumentJSRef = JSValueToStringCopy(ctx, arguments[i], NULL);
//            size_t length = JSStringGetLength(msgArgumentJSRef) + 1;
//            std::unique_ptr<char[]> stringBuffer = std::make_unique<char[]>(length);
//            JSStringGetUTF8CString(msgArgumentJSRef, stringBuffer.get(), length);
//            //ultralight::String str(stringBuffer.get(), length);
//            std::string str(stringBuffer.get());
//            strs[i] = str;
//
//        }
//    }
//
//    global_MyApp->ChangeRoom_CPP(strs[0], strs[1]);
//
//    delete[] strs;
//
//    return JSValueMakeNull(ctx);
//}

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
    global_client->SendMessageToServer(packaged);

    delete[] strs;

    return JSValueMakeNull(ctx);
}

JSValueRef cpp_create_room(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("cpp_create_room() called"));

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
    
    global_MyApp->create_room_cpp(room_name);

    return JSValueMakeNull(ctx);
}

// This callback will be bound to 'CPPLogin()' on the page.
//JSValueRef CPPOpenDashboard(JSContextRef ctx, JSObjectRef function,
//    JSObjectRef thisObject, size_t argumentCount,
//    const JSValueRef arguments[], JSValueRef* exception) {
//
//    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CPPOpenDashboard() called"));
//
//    global_MyApp->clear_current_room();
//    global_MyApp->open_dashboard();
//
//    return JSValueMakeNull(ctx);
//}

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

    //FString_Packet p = { ECommand::Authorized, "james" };
    //global_MyApp->UserLoggedIn(p);

    return JSValueMakeNull(ctx);
}

JSValueRef CPPSubmitMessage(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CPPSubmitMessage"));

    std::string message = "";

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
            message = str;

        }
    }
    if (global_MyApp->get_current_room() != nullptr)
    {
        FPost_Message_Packet packet = { ECommand::Post, ESub_Command::Message, global_MyApp->get_current_room()->Get_ID(), message };

        std::string packet_string = PacketDecoder::Post_Message_Packet_To_String(packet);

        global_client->SendMessageToServer(packet_string);
    }
    
    
    

    return JSValueMakeNull(ctx);
}

//JSValueRef cpp_open_create_room_panel(JSContextRef ctx, JSObjectRef function,
//    JSObjectRef thisObject, size_t argumentCount,
//    const JSValueRef arguments[], JSValueRef* exception) {
//
//    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("open_create_room_panel() called"));
//
//    global_MyApp->open_create_room_panel();
//    return JSValueMakeNull(ctx);
//}

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
        JSStringRef name = JSStringCreateWithUTF8CString("CPPSubmitMessage");

        // Create a garbage-collected JavaScript function that is bound to our
        // native C callback 'OnButtonClick()'.
        JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
            CPPSubmitMessage);

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

    if (caller == side_panel->view())
    {
        side_panel_view.DOMLoaded = true;
        side_panel_view.Update();
        side_panel_view.BindJavaScriptFunctions();
        //ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
        //// Get the underlying JSContextRef for use with the
        //// JavaScriptCore C API.
        //JSContextRef ctx = context->ctx();
        //
        //// Create a JavaScript String containing the name of our callback.
        //JSStringRef name = JSStringCreateWithUTF8CString("open_dashboard");
        //
        //// Create a garbage-collected JavaScript function that is bound to our
        //// native C callback 'OnButtonClick()'.
        //JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
        //    CPPOpenDashboard);
        //
        //// Get the global JavaScript object (aka 'window')
        //JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
        //
        //// Store our function in the page's global JavaScript object so that it
        //// accessible from the page as 'OnButtonClick()'.
        //JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
        //
        //// Release the JavaScript String we created earlier.
        //JSStringRelease(name);
        //
        //// Create a JavaScript String containing the name of our callback.
        //JSStringRef name_create_room = JSStringCreateWithUTF8CString("open_create_room_panel");
        //
        //// Create a garbage-collected JavaScript function that is bound to our
        //// native C callback 'OnButtonClick()'.
        //JSObjectRef func_open_create_room = JSObjectMakeFunctionWithCallback(ctx, name_create_room,
        //    cpp_open_create_room_panel);
        //
        //// Store our function in the page's global JavaScript object so that it
        //// accessible from the page as 'OnButtonClick()'.
        //JSObjectSetProperty(ctx, globalObj, name_create_room, func_open_create_room, 0, 0);
        //
        //// Release the JavaScript String we created earlier.
        //JSStringRelease(name_create_room);
        
    }

    if (caller == dashboard_panel->view())
    {
        dashboard_panel_view.DOMLoaded = true;
        dashboard_panel_view.Update();
        dashboard_panel_view.BindJavaScriptFunctions();
        //ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
        //// Get the underlying JSContextRef for use with the
        //// JavaScriptCore C API.
        //JSContextRef ctx = context->ctx();
        //
        //// Create a JavaScript String containing the name of our callback.
        //JSStringRef name = JSStringCreateWithUTF8CString("ChangeRoom");
        //
        //// Create a garbage-collected JavaScript function that is bound to our
        //// native C callback 'OnButtonClick()'.
        //JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
        //    ChangeRoom);
        //
        //// Get the global JavaScript object (aka 'window')
        //JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
        //
        //// Store our function in the page's global JavaScript object so that it
        //// accessible from the page as 'OnButtonClick()'.
        //JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
        //
        //// Release the JavaScript String we created earlier.
        //JSStringRelease(name);
    }
   
    if (caller == create_room_panel->view())
    {
        ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
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

        create_room_panel->Hide();
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

std::vector<FPost_Message_Packet> queue = std::vector<FPost_Message_Packet>();
void MyApp::Post_Room_Meassage(FPost_Message_Packet _packet)
{
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("Post_Room_Message()"));
    std::unique_lock<std::mutex>lock(Message_Queue_Mutex);
    queue.push_back(_packet);
    b_new_message = true;
}

void MyApp::CheckQueue()
{
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CheckQueue"));
    std::lock_guard<std::mutex>lock(Message_Queue_Mutex);
    if (queue.size() > 0)
    {
        for (std::vector<FPost_Message_Packet>::iterator it = queue.begin(); it < queue.end(); it++)
        {
            FPost_Message_Packet packet = *it;
            if (active_room_exists(packet.Room_ID))
            {
                Room* room = get_active_room(packet.Room_ID);
                if (room)
                {
                    room->add_message(packet.Content);

                    if (current_active_room)
                    {
                        if (current_active_room->Get_ID() == room->Get_ID())
                        {
                            add_message_room(packet.Content);
                        }
                    }
                }
            }
        }
        
        queue.clear();
        ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("CheckQueue(): clear queue"));
    }
    //Message_Queue_cv.notify_one();
    b_new_message = false;
}

void MyApp::UserLoggedIn(FString_Packet _packet)
{
    // pass it to HTML_View_Sidepanel class
    side_panel_view.SetUsername(_packet._string);
    user.Username = _packet._string;
    isAuthenticated = true;

    open_dashboard();
}

void MyApp::ChangeRoom_CPP(std::string _room_id, std::string _room_name)
{
   //ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("ChangeRoom_CPP"));
   //int com = (int)ECommand::Get;
   //std::string com_string = std::to_string(com);
   //
   //int sub_com = (int)ESub_Command::Room;
   //std::string sub_com_string = std::to_string(sub_com);
   //
   //std::string packetString = com_string + ";" + sub_com_string + ";" + _room;
   //
   //clientNetworking->SendMessageToServer(packetString);

    Room* room = get_active_room(std::stoi(_room_id));
    if (room)
    {
        if (current_active_room != nullptr)
        {
            clear_current_room();
        }

        current_active_room = room;

        for (std::vector<std::string>::iterator it = current_active_room->messages.begin(); it < current_active_room->messages.end(); it++)
        {
            add_message_room(*it);
        }
    }
    else
    {
        int room_id = std::stoi(_room_id);
        activate_room(room_id, _room_name);
        room = get_active_room(std::stoi(_room_id));
        if (room)
        {
            if (current_active_room != nullptr)
            {
                clear_current_room();
            }

            current_active_room = room;

            for (std::vector<std::string>::iterator it = current_active_room->messages.begin(); it < current_active_room->messages.end(); it++)
            {
                add_message_room(*it);
            }
        }
    }
    if (!dashboard_panel->is_hidden())
    {
        dashboard_panel->Hide();
    }
    if (room_panel->is_hidden())
    {
        room_panel->Show();
    }
}

void MyApp::create_room_cpp(std::string _name)
{
    FPost_Room_Packet packet = { ECommand::Post, ESub_Command::Room, 0, _name };

    std::string packet_string = PacketDecoder::Post_Room_Packet_To_String(packet);

    clientNetworking->SendMessageToServer(packet_string);
}

void MyApp::AddRoom(FPost_Room_Packet _packet)
{
    std::lock_guard<std::mutex>lock(post_room_mutex);

    for (std::vector<FPost_Room_Packet>::iterator it = post_rooms.begin(); it != post_rooms.end(); it++)
    {
        _packet = *it;

        //activate_room(_packet.Room_ID, _packet.Room_Name);

        ultralight::String id_string(std::to_string(_packet.Room_ID).c_str());
        ultralight::String room_string(_packet.Room_Name.c_str());
        ultralight::String func_start = "AddRoomButton('";
        ultralight::String func_params = id_string + "', '" + room_string + "'";
        ultralight::String func_end = ")";

        ultralight::String func_string = func_start + func_params + func_end;

        dashboard_panel->view().get()->EvaluateScript(func_string);
        dashboard_panel->view().get()->set_needs_paint(true);
    }

    post_rooms.clear();
    b_addroom = false;
    
}

void MyApp::post_room(FPost_Room_Packet _packet)
{
    std::lock_guard<std::mutex>lock(post_room_mutex);
    post_rooms.push_back(_packet);
    b_addroom = true;
}

bool MyApp::active_room_exists(int _room_id)
{
    for (std::vector<Room*>::iterator it = active_rooms.begin(); it < active_rooms.end(); it++)
    {
        Room* room = *it;
        if (room->Get_ID() == _room_id)
        {
            return true;
        }
    }
    return false;
}

Room* MyApp::get_active_room(int _id)
{
    for (std::vector<Room*>::iterator it = active_rooms.begin(); it < active_rooms.end(); it++)
    {
        Room* room = *it;
        if (room->Get_ID() == _id)
        {
            return *it;
        }
    }
    return nullptr;
}

void MyApp::open_dashboard()
{
    ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("open_dashboard()"));
    if (!login_panel->is_hidden())
    {
        login_panel->Hide();
    }
    if (!room_panel->is_hidden())
    {
        room_panel->Hide();
    }
    if (!create_room_panel->is_hidden())
    {
        create_room_panel->Hide();
    }
    if (dashboard_panel->is_hidden())
    {
        dashboard_panel->Show();
    }
}

void MyApp::activate_room(int _id, std::string _name)
{
    if (active_room_exists(_id)) { return; }
    Room* new_room = new Room(_id, _name, this, clientNetworking);
    active_rooms.push_back(new_room);
}

void MyApp::add_message_room(std::string _message)
{
    std::string msgs = _message;
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

void MyApp::clear_current_room()
{
    room_panel->view().get()->EvaluateScript("ClearRoom()");
}

void MyApp::open_create_room_panel()
{
    if (dashboard_panel->is_hidden() == false)
    {
        dashboard_panel->Hide();
    }
    if (room_panel->is_hidden() == false)
    {
        room_panel->Hide();
    }
    if (login_panel->is_hidden() == false)
    {
        login_panel->Hide();
    }

    if (create_room_panel->is_hidden())
    {
        create_room_panel->Show();
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