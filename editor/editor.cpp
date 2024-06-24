#include "pch.h"
#include <editor/editor.h>
#include "app.h"

editor::IAppPtr editor::CreateApp()
{
	return App::Instance();
}