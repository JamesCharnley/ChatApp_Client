#pragma once
#include "HTML_View.h"
#include <iostream>
class HTML_View_Login_Panel :
    public HTML_View
{
public:

    HTML_View_Login_Panel();
    HTML_View_Login_Panel(View* _overlay, RefPtr<Overlay> _ref, class MyApp* _myApp, class Client* _client);

    virtual void Resize(ultralight::Window* window, uint32_t width, uint32_t height) override;

    virtual void BindJavaScriptFunctions() override;

};
