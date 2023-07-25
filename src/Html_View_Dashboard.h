#pragma once
#include "HTML_View.h"
#include <iostream>
class HTML_View_Dashboard :
    public HTML_View
{
public:

    HTML_View_Dashboard();
    HTML_View_Dashboard(View* _overlay, RefPtr<Overlay> _ref, class MyApp* _myApp);
    
    virtual void Resize(ultralight::Window* window, uint32_t width, uint32_t height) override;

    virtual void BindJavaScriptFunctions() override;

};