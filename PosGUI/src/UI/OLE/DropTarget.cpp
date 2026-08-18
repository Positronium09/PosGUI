module;
#include <Windows.h>

#include <ole2.h>

module PGUI.UI.OLE.DragDrop.DropTarget;

import PGUI.ErrorHandling;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.OLE.DataObject;
import PGUI.UI.OLE.DragDrop.DragDropEnums;
import PGUI.UI.OLE.DragDrop.DragDropStructs;

import std;

namespace PGUI::UI::OLE::DragDrop
{
	DropTarget::DropTarget(DragEnterCallback dragEnterCallback, DragLeaveCallback dragLeaveCallback,
	                       DragOverCallback dragOverCallback, DropCallback dropCallback) noexcept :
		dragEnterCallback{ MoveChecked(dragEnterCallback) },
		dragLeaveCallback{ MoveChecked(dragLeaveCallback) },
		dragOverCallback{ MoveChecked(dragOverCallback) },
		dropCallback{ MoveChecked(dropCallback) }
	{ }

	auto DropTarget::DragEnter(IDataObject* obj, DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT
	{
		if (effect == nullptr || obj == nullptr)
		{
			return E_POINTER;
		}

		const auto keyState = KeyState{
			.shift = static_cast<bool>(keyStateFlag & MK_SHIFT),
			.control = static_cast<bool>(keyStateFlag & MK_CONTROL),
			.alt = static_cast<bool>(keyStateFlag & MK_ALT),
			.lButton = static_cast<bool>(keyStateFlag & MK_LBUTTON),
			.mButton = static_cast<bool>(keyStateFlag & MK_MBUTTON),
			.rButton = static_cast<bool>(keyStateFlag & MK_RBUTTON)
		};

		const auto result = dragEnterCallback(DataObjectReadWrite{ obj }, keyState, PointL{ point.x, point.y });

		if (!result.has_value())
		{
			*effect = DROPEFFECT_NONE;
			return result.error().HResult();
		}

		*effect = ToUnderlying<DWORD>(result.value());
		return S_OK;
	}

	auto DropTarget::DragLeave() -> HRESULT
	{
		if (const auto result = dragLeaveCallback();
			!result.has_value())
		{
			return result.error().HResult();
		}

		return S_OK;
	}

	auto DropTarget::DragOver(DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT
	{
		if (effect == nullptr)
		{
			return E_POINTER;
		}

		const auto keyState = KeyState{
			.shift = static_cast<bool>(keyStateFlag & MK_SHIFT),
			.control = static_cast<bool>(keyStateFlag & MK_CONTROL),
			.alt = static_cast<bool>(keyStateFlag & MK_ALT),
			.lButton = static_cast<bool>(keyStateFlag & MK_LBUTTON),
			.mButton = static_cast<bool>(keyStateFlag & MK_MBUTTON),
			.rButton = static_cast<bool>(keyStateFlag & MK_RBUTTON)
		};

		const auto result = dragOverCallback(keyState, PointL{ point.x, point.y });

		if (!result.has_value())
		{
			*effect = DROPEFFECT_NONE;
			return result.error().HResult();
		}

		*effect = ToUnderlying<DWORD>(result.value());
		return S_OK;
	}

	auto DropTarget::Drop(IDataObject* obj, DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT
	{
		if (effect == nullptr || obj == nullptr)
		{
			return E_POINTER;
		}

		const auto keyState = KeyState{
			.shift = static_cast<bool>(keyStateFlag & MK_SHIFT),
			.control = static_cast<bool>(keyStateFlag & MK_CONTROL),
			.alt = static_cast<bool>(keyStateFlag & MK_ALT),
			.lButton = static_cast<bool>(keyStateFlag & MK_LBUTTON),
			.mButton = static_cast<bool>(keyStateFlag & MK_MBUTTON),
			.rButton = static_cast<bool>(keyStateFlag & MK_RBUTTON)
		};

		const auto result = dropCallback(DataObjectReadWrite{ obj }, keyState, PointL{ point.x, point.y });

		if (!result.has_value())
		{
			*effect = DROPEFFECT_NONE;
			return result.error().HResult();
		}

		*effect = ToUnderlying<DWORD>(result.value());
		return S_OK;
	}
}
