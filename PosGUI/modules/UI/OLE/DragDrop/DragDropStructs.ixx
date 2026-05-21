module;
#include <Windows.h>

#include <OleIdl.h>

export module PGUI.UI.OLE.DragDrop.DragDropStructs;

export namespace PGUI::UI::OLE::DragDrop
{
	struct KeyState
	{
		bool shift : 1 = false;
		bool control : 1 = false;
		bool alt : 1 = false;
		bool lButton : 1 = false;
		bool mButton : 1 = false;
		bool rButton : 1 = false;
	};
}
