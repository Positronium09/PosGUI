module;
#include <Windows.h>

#include <ole2.h>

export module PGUI.UI.OLE.DragDrop.DropTarget;

import PGUI.ErrorHandling;
import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.UI.OLE.DataObject;
import PGUI.UI.OLE.DragDrop.DragDropEnums;
import PGUI.UI.OLE.DragDrop.DragDropStructs;

import std;

export namespace PGUI::UI::OLE::DragDrop
{
	class DropTarget final : public Implements<DropTarget, IDropTarget>
	{
		public:
		using DragEnterCallback = std::move_only_function<Result<DropEffect>(const DataObjectReadWrite&, KeyState, PointL) noexcept>;
		using DragLeaveCallback = std::move_only_function<Result<void>() noexcept>;
		using DragOverCallback = std::move_only_function<Result<DropEffect>(KeyState, PointL) noexcept>;
		using DropCallback = std::move_only_function<Result<DropEffect>(const DataObjectReadWrite&, KeyState, PointL) noexcept>;

		DropTarget(DragEnterCallback dragEnterCallback, 
			DragLeaveCallback dragLeaveCallback, 
			DragOverCallback dragOverCallback,
			DropCallback dropCallback) noexcept;

		auto DragEnter(IDataObject* obj, DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT override;
		auto DragLeave() -> HRESULT override;
		auto DragOver(DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT override;
		auto Drop(IDataObject* obj, DWORD keyStateFlag, POINTL point, DWORD* effect) -> HRESULT override;

		private:
		DragEnterCallback dragEnterCallback;
		DragLeaveCallback dragLeaveCallback;
		DragOverCallback dragOverCallback;
		DropCallback dropCallback;
	};
}
