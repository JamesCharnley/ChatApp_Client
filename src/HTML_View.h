#pragma once
#include <AppCore/Overlay.h>


using namespace ultralight;

class HTML_View
{
public:
	HTML_View();
	HTML_View(View* _overlay);

	virtual void Update();
	
	View* GetOverlay();

	bool DOMLoaded = false;

	void TryUpdate();

protected:

	View* overlay;
};

