#include "HTML_View.h"

HTML_View::HTML_View(View* _overlay)
{
	overlay = _overlay;
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

HTML_View::HTML_View()
{
}
