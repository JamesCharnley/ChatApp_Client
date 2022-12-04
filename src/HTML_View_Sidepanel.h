#pragma once
#include "HTML_View.h"
#include <iostream>
class HTML_View_Sidepanel :
    public HTML_View
{
public:
    HTML_View_Sidepanel();
    HTML_View_Sidepanel(View* _overlay);

    void Update() override;

    void SetUsername(std::string _username);

private:

    void UpdateUsername(std::string _username);

private:

    std::string username = "placeholder";

};

