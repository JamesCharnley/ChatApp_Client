#pragma once
#include "HTML_View.h"
#include <iostream>
class HTML_View_Sidepanel :
    public HTML_View
{
public:
    HTML_View_Sidepanel();
    HTML_View_Sidepanel(View* _overlay, RefPtr<Overlay> _ref, class MyApp* _myApp);

    void Update() override;

    void SetUsername(std::string _username);

    virtual void Resize(ultralight::Window* window, uint32_t width, uint32_t height) override;

    virtual void BindJavaScriptFunctions() override;

private:

    void UpdateUsername(std::string _username);

private:

    std::string username = "placeholder";

    

};

