module;
#include <Windows.h>

#include <ole2.h>

export module PGUI.UI.OLE.DragDrop.DropSource;

import PGUI.ErrorHandling;
import PGUI.ComPtr;
import PGUI.UI.OLE.DataObject;
import PGUI.UI.OLE.DragDrop.DragDropEnums;
import PGUI.UI.OLE.DragDrop.DragDropStructs;

import std;

export namespace PGUI::UI::OLE::DragDrop
{
	class DropSource final : public Implements<DropSource, IDropSource>
	{
		public:
		using GiveFeedbackCallback = std::move_only_function<Result<bool>(DropEffect) noexcept>;
		using QueryContinueDragCallback = std::move_only_function<Result<ContinueDragResult>(bool, KeyState) noexcept>;

		DropSource(GiveFeedbackCallback giveFeedbackCallback, QueryContinueDragCallback queryContinueDragCallback) noexcept;

		auto GiveFeedback(DWORD effect) -> HRESULT override;
		auto QueryContinueDrag(BOOL escapePressed, DWORD keyStateFlag) -> HRESULT override;

		private:
		GiveFeedbackCallback giveFeedbackCallback;
		QueryContinueDragCallback queryContinueDragCallback;
	};
}
