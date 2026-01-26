module;
#include <Windows.h>

export module PGUI.UI.DragDrop.DragDropTypes;

import PGUI.UI.Input;

export namespace PGUI::UI::DragDrop
{
	struct DragDropModifierInfo
	{
		ModifierKeys modifierKeys;
		MouseButton mouseButton;
	};

	enum class DragDropEffect : DWORD
	{
		None = DROPEFFECT_NONE,
		Copy = DROPEFFECT_COPY,
		Move = DROPEFFECT_MOVE,
		Link = DROPEFFECT_LINK,
		Scroll = DROPEFFECT_SCROLL,
		All = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK
	};
	DEFINE_ENUM_FLAG_OPERATORS(DragDropEffect);

	using ClipFormat = CLIPFORMAT;

	struct FormatETC
	{
		ClipFormat clipFormat;

	};
}
