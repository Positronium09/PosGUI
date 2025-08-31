export module PGUI.UI.UIBase:UIContainer;

import std;

import :Interface;
import PGUI.Event;
import PGUI.ErrorHandling;
import PGUI.Shape2D;

export namespace PGUI::UI
{
	class UIContainer
	{
		public:
		EventSRWM<const UIComponentPtr<>&, std::size_t> childAdded;
		EventSRWM<const UIComponentPtr<>&, std::size_t> childRemoved;
		EventSRWM<const UIComponentPtr<>&, std::size_t, std::size_t> childMoved;
		EventSRWM<> childrenCleared;


	};
}
