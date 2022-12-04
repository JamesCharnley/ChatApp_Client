#include "HTML_View_Sidepanel.h"
#include "MyApp.h"

HTML_View_Sidepanel::HTML_View_Sidepanel()
{
}

HTML_View_Sidepanel::HTML_View_Sidepanel(View* _overlay) : HTML_View::HTML_View(_overlay)
{
	username = "";
}

void HTML_View_Sidepanel::Update()
{
	UpdateUsername(username);
}



void HTML_View_Sidepanel::SetUsername(std::string _username)
{
	username = _username;

	if (DOMLoaded == true)
	{
		Update();
	}
}

//if (DOMLoaded == true)
//{
//	Update();
//}

void HTML_View_Sidepanel::UpdateUsername(std::string _username)
{
	
	String ul_string(_username.c_str());

	ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, ul_string);
	
	String funcString = "";
	String funcStart = "SetUsername('";
	String funcParam = ul_string;
	String funcEnd = "')";

	funcString = funcStart + funcParam + funcEnd;
	
	overlay->EvaluateScript(funcString);
	
}

//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("b4 evaluate script"));
//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, funcString);
//
//ultralight::GetDefaultLogger("C:/Users/James/AppData/Roaming/MyCompany/MyApp/default/ultralight.log")->LogMessage(ultralight::LogLevel::Info, String("after evaluate script"));