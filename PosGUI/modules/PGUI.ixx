module;
#include <Windows.h>

export module PGUI;

export import PGUI.Window;
export import PGUI.WindowClass;
export import PGUI.MessageLoop;
export import PGUI.EnumFlag;
export import PGUI.Event;
export import PGUI.Exceptions;
export import PGUI.IEnumStringIterator;
export import PGUI.Logging;
export import PGUI.ComPtr;
export import PGUI.DataBinding;
export import PGUI.Factories;
export import PGUI.Shape2D;
export import PGUI.Utils;
export import PGUI.UI;


export namespace PGUI
{
	void Init();
}
