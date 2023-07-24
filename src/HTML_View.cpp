#include "HTML_View.h"

HTML_View::HTML_View(View* _overlay, RefPtr<Overlay> _ref, MyApp* _myApp)
{
	overlay = _overlay;
	ref = _ref;

}

void HTML_View::Update()
{
}

View* HTML_View::GetOverlay()
{
	return overlay;
}

void HTML_View::TryUpdate()
{
	if (DOMLoaded)
	{
		Update();
	}
}

void HTML_View::BindJavaScriptFunctions()
{
}

void HTML_View::Resize(ultralight::Window* window, uint32_t width, uint32_t height)
{
	ref->Resize(width, height);
}

HTML_View::HTML_View()
{
}
