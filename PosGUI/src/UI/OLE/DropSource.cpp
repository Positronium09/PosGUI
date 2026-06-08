module;
#include <Windows.h>

#include <ole2.h>

module PGUI.UI.OLE.DragDrop.DropSource;

import PGUI.ErrorHandling;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.UI.OLE.DataObject;
import PGUI.UI.OLE.DragDrop.DragDropEnums;
import PGUI.UI.OLE.DragDrop.DragDropStructs;

import std;

namespace PGUI::UI::OLE::DragDrop
{
	DropSource::DropSource(GiveFeedbackCallback giveFeedbackCallback,
	                       QueryContinueDragCallback queryContinueDragCallback) noexcept :
		giveFeedbackCallback{ MoveChecked(giveFeedbackCallback) },
		queryContinueDragCallback{ MoveChecked(queryContinueDragCallback) }
	{ }

	auto DropSource::GiveFeedback(DWORD effect) -> HRESULT
	{
		const auto result = giveFeedbackCallback(FromUnderlying<DropEffect>(effect));
		if (!result.has_value())
		{
			return result.error().HResult();
		}

		if (result.value())
		{
			return DRAGDROP_S_USEDEFAULTCURSORS;
		}

		return S_OK;
	}

	auto DropSource::QueryContinueDrag(BOOL escapePressed, DWORD keyStateFlag) -> HRESULT
	{
		const auto keyState = KeyState{
			.shift = static_cast<bool>(keyStateFlag & MK_SHIFT),
			.control = static_cast<bool>(keyStateFlag & MK_CONTROL),
			.alt = static_cast<bool>(keyStateFlag & MK_ALT),
			.lButton = static_cast<bool>(keyStateFlag & MK_LBUTTON),
			.mButton = static_cast<bool>(keyStateFlag & MK_MBUTTON),
			.rButton = static_cast<bool>(keyStateFlag & MK_RBUTTON)
		};

		const auto result = queryContinueDragCallback(escapePressed, keyState);
		if (!result.has_value())
		{
			return result.error().HResult();
		}

		switch (const auto& continueDragResult = result.value();
			continueDragResult)
		{
			case ContinueDragResult::Continue:
			{
				return S_OK;
			}
			case ContinueDragResult::Drop:
			{
				return DRAGDROP_S_DROP;
			}
			case ContinueDragResult::Cancel:
			{
				return DRAGDROP_S_CANCEL;
			}
		}

		std::unreachable();
	}
}
