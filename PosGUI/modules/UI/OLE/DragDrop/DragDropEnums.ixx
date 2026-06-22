module;
#include <Windows.h>

#include <OleIdl.h>

export module PGUI.UI.OLE.DragDrop.DragDropEnums;

import PGUI.Utils;

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
	consteval auto MakeEnumFlag(DropEffect) noexcept -> void { }

	enum class ContinueDragResult
	{
		Continue,
		Drop,
		Cancel
	};
}
