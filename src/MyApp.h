#pragma once
#include <AppCore/AppCore.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include "HTML_View_Sidepanel.h"
#include "PacketDecoder.h"
#include "Client.h"

using namespace ultralight;

class MyApp : public AppListener,
              public WindowListener,
              public LoadListener,
              public ViewListener {
public:

    char testchar[7];

  MyApp();

  virtual ~MyApp();

  // Start the run loop.
  virtual void Run();

  // This is called continuously from the app's main loop.
  virtual void OnUpdate() override;

  // This is called when the window is closing.
  virtual void OnClose(ultralight::Window* window) override;

  // This is called whenever the window resizes.
  virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override;

  // This is called when the page finishes a load in one of its frames.
  virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) override;

  // This is called when the DOM has loaded in one of its frames.
  virtual void OnDOMReady(ultralight::View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) override;

  // This is called when the page requests to change the Cursor.
  virtual void OnChangeCursor(ultralight::View* caller,
    Cursor cursor) override;

  virtual void OnChangeTitle(ultralight::View* caller,
    const String& title) override;

  void UpdateElement(std::string _message);
  void CheckQueue();
  Client* clientNetworking;

  std::vector<std::string> messages = std::vector<std::string>();

  std::mutex Message_Queue_Mutex;
  std::condition_variable Message_Queue_cv;

  void UserLoggedIn(FString_Packet _packet);
  bool b_UserLoggedIn = false;
  std::string username = "";

protected:
  RefPtr<App> app_;
  RefPtr<Window> window_;
  HTML_View_Sidepanel side_panel_view;
  RefPtr<Overlay> side_panel;
  RefPtr<Overlay> room_panel;
  RefPtr<Overlay> login_panel;
  RefPtr<Overlay> dashboard_panel;

  bool isAuthenticated = false;

  std::vector<ultralight::View*> callers = std::vector< ultralight::View*>();

  FUser user;
  //JSValueRef CPPLogin(JSContextRef ctx, JSObjectRef function,
  //    JSObjectRef thisObject, size_t argumentCount,
  //    const JSValueRef arguments[], JSValueRef* exception);

};
