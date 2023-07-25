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
#include "HTML_View_Dashboard.h"
#include "HTML_View_Login_Panel.h"
#include "PacketDecoder.h"
#include "Client.h"
#include "Room.h"

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

  void Post_Room_Meassage(FPost_Message_Packet _packet);
  void CheckQueue();
  Client* clientNetworking;

  std::vector<std::string> messages = std::vector<std::string>();

  std::mutex Message_Queue_Mutex;
  std::condition_variable Message_Queue_cv;

  void UserLoggedIn(FString_Packet _packet);
  bool b_UserLoggedIn = false;
  std::string username = "";

  void ChangeRoom_CPP(std::string _room_id, std::string _room_name);

  void create_room_cpp(std::string _name);

  void AddRoom(FPost_Room_Packet _packet);

  View* get_db() { return dashboard_panel->view().get(); };

  bool b_addroom = false;
  void post_room(FPost_Room_Packet _packet);
  std::mutex post_room_mutex;
  std::vector<FPost_Room_Packet> post_rooms = std::vector<FPost_Room_Packet>();

  bool active_room_exists(int _room_id);

  Room* get_active_room(int _id);

  Room* get_current_room() { return current_active_room; };

  void open_dashboard();

  void clear_current_room();

  void open_create_room_panel();
protected:
  RefPtr<App> app_;
  RefPtr<Window> window_;
  HTML_View_Sidepanel side_panel_view;
  RefPtr<Overlay> side_panel;
  RefPtr<Overlay> room_panel;

  HTML_View_Login_Panel login_panel_view;
  RefPtr<Overlay> login_panel;

  HTML_View_Dashboard dashboard_panel_view;
  RefPtr<Overlay> dashboard_panel;
  RefPtr<Overlay> create_room_panel;

  std::vector<HTML_View*> htmlViews = std::vector<HTML_View*>();

  bool isAuthenticated = false;

  std::vector<ultralight::View*> callers = std::vector< ultralight::View*>();

  std::vector<Room*> active_rooms = std::vector<Room*>();

  void activate_room(int _id, std::string _name);

  FUser user;
  //JSValueRef CPPLogin(JSContextRef ctx, JSObjectRef function,
  //    JSObjectRef thisObject, size_t argumentCount,
  //    const JSValueRef arguments[], JSValueRef* exception);

  Room* current_active_room = nullptr;

  bool b_new_message = false;

  void add_message_room(std::string _message);

};
