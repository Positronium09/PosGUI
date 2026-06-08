module;
#include <Windows.h>

#include <ole2.h>

export module PGUI.UI.OLE;

export import PGUI.UI.OLE.DataObject;
export import PGUI.UI.OLE.EnumFormatData;
export import PGUI.UI.OLE.OLEEnums;
export import PGUI.UI.OLE.OLEStructs;
export import PGUI.UI.OLE.Clipboard;
export import PGUI.UI.OLE.DragDrop;

import PGUI.Utils;
import PGUI.Window;
import PGUI.ErrorHandling;

export namespace PGUI::UI::OLE::DragDrop
{
	[[nodiscard]] auto RevokeDragDropOperation(const WindowPtr<const Window>& window) -> Result<void>
	{
		if (const auto error = Error{ RevokeDragDrop(window->Hwnd()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}
	
	[[nodiscard]] auto RegisterDragDropOperation(const WindowPtr<const Window>& window, DropTarget& target) -> Result<void>
	{
		if (const auto error = Error{ RegisterDragDrop(window->Hwnd(), &target) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	struct DoDragDropResult
	{
		DropEffect effect = DropEffect::None;
		bool dropped = false;
	};

	template <DataObjectHandler Handler, bool AllowExtraFormats>
	[[nodiscard]] auto DoDragDropOperation(
		DataObject<Handler, AllowExtraFormats>& dataObject,
		DropSource& source,
		const DropEffect dropEffect) -> Result<DoDragDropResult>
	{
		DWORD effectOut{ DROPEFFECT_NONE };

		const auto error = Error{ DoDragDrop(
			&dataObject,
			&source,
			ToUnderlying<DWORD>(dropEffect),
			&effectOut) };

		if (error.IsFailure())
		{
			return Unexpected{ error };
		}

		return DoDragDropResult{
			.effect = FromUnderlying<DropEffect>(effectOut),
			.dropped = error.HResult() == DRAGDROP_S_DROP
		};
	}

	[[nodiscard]] auto DoDragDropOperation(
		DataObjectReadWrite& dataObject,
		DropSource& source,
		const DropEffect dropEffect) -> Result<DoDragDropResult>
	{
		DWORD effectOut{ DROPEFFECT_NONE };

		const auto error = Error{ DoDragDrop(
			dataObject.GetRaw(),
			&source,
			ToUnderlying<DWORD>(dropEffect),
			&effectOut) };

		if (error.IsFailure())
		{
			return Unexpected{ error };
		}

		return DoDragDropResult{
			.effect = FromUnderlying<DropEffect>(effectOut),
			.dropped = error.HResult() == DRAGDROP_S_DROP
		};
	}
}
