#pragma once
#include <AppCore/Overlay.h>


using namespace ultralight;

class HTML_View
{
public:
	HTML_View();
	HTML_View(View* _overlay, RefPtr<Overlay> _ref, class MyApp* _myApp);

	virtual void Update();
	
	View* GetOverlay();

	bool DOMLoaded = false;

	void TryUpdate();

	virtual void BindJavaScriptFunctions();

	virtual void Resize(ultralight::Window* window, uint32_t width, uint32_t height);

protected:

	View* overlay;
	RefPtr<Overlay> ref;
};

