export module PGUI;

export import PGUI.Window;
export import PGUI.WindowClass;
export import PGUI.MessageLoop;
export import PGUI.Event;
export import PGUI.Mutex;
export import PGUI.ErrorHandling;
export import PGUI.IEnumStringIterator;
export import PGUI.ComPtr;
export import PGUI.ScopedTimer;
export import PGUI.DataBinding;
export import PGUI.Factories;
export import PGUI.Shape2D;
export import PGUI.Utils;
export import PGUI.UI;

export namespace PGUI
{
	auto Init() -> void;
}
