module;
#include <Windows.h>

#include <OleIdl.h>

export module PGUI.UI.OLE.DragDrop.DragDropEnums;

export namespace PGUI::UI::OLE::DragDrop
{
	enum class DropEffect : DWORD
	{
		None = DROPEFFECT_NONE,
		Copy = DROPEFFECT_COPY,
		Move = DROPEFFECT_MOVE,
		Link = DROPEFFECT_LINK,
		Scroll = DROPEFFECT_SCROLL
	};
	DEFINE_ENUM_FLAG_OPERATORS(DropEffect);

	enum class ContinueDragResult
	{
		Continue,
		Drop,
		Cancel
	};
}
