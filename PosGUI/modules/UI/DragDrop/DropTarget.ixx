module;
#include <ole2.h>

export module PGUI.UI.DragDrop.DropTarget;

import PGUI.Event;
import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.Input;
import PGUI.UI.DragDrop.DragDropTypes;

export namespace PGUI::UI::DragDrop
{
	class DropTarget : public Implements<DropTarget, IDropTarget>
	{
		public:
		DropTarget() = default;
		~DropTarget() noexcept override = default;

		virtual auto OnDragEnter(ComPtr<IDataObject> dataObj,
			DragDropModifierInfo modifierInfo,
			PointF point, DragDropEffect& effect) -> void = 0;
		virtual auto OnDragOver(DragDropModifierInfo modifierInfo,
			PointF point, DragDropEffect& effect) -> void = 0;
		virtual auto OnDragLeave() -> void = 0;
		virtual auto OnDrop(ComPtr<IDataObject> dataObj,
			DragDropModifierInfo modifierInfo,
			PointF point, DragDropEffect& effect) -> void = 0;

		private:
		auto DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) -> HRESULT override;
		auto DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) -> HRESULT override;
		auto DragLeave() -> HRESULT override;
		auto Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) -> HRESULT override;
	};

	class DropTargetEvent final : public DropTarget
	{
		public:
		DropTargetEvent() = default;

		[[nodiscard]] auto& DragEnterEvent() noexcept { return dragEnterEvent; }
		[[nodiscard]] const auto& DragEnterEvent() const noexcept { return dragEnterEvent; }
		[[nodiscard]] auto& DragLeaveEvent() noexcept { return dragLeaveEvent; }
		[[nodiscard]] const auto& DragLeaveEvent() const noexcept { return dragLeaveEvent; }
		[[nodiscard]] auto& DragOverEvent() noexcept { return dragOverEvent; }
		[[nodiscard]] const auto& DragOverEvent() const noexcept { return dragOverEvent; }
		[[nodiscard]] auto& DropEvent() noexcept { return dropEvent; }
		[[nodiscard]] const auto& DropEvent() const noexcept { return dropEvent; }

		auto OnDragEnter(const ComPtr<IDataObject> dataObj,
			const DragDropModifierInfo modifierInfo,
			const PointF point, DragDropEffect& effect) -> void override
		{
			dragEnterEvent.Invoke(dataObj, modifierInfo, point, effect);
		}
		auto OnDragOver(const DragDropModifierInfo modifierInfo,
			const PointF point, DragDropEffect& effect) -> void override
		{
			dragOverEvent.Invoke(modifierInfo, point, effect);
		}
		auto OnDragLeave() -> void override
		{
			dragLeaveEvent.Invoke();
		}
		auto OnDrop(const ComPtr<IDataObject> dataObj,
			const DragDropModifierInfo modifierInfo,
			const PointF point, DragDropEffect& effect) -> void override
		{
			dropEvent.Invoke(dataObj, modifierInfo, point, effect);
		}

		private:
		EventSRWM<
			ComPtr<IDataObject>,
			DragDropModifierInfo,
			PointF, DragDropEffect&> dragEnterEvent;
		EventSRWM<> dragLeaveEvent;
		EventSRWM<
			DragDropModifierInfo,
			PointF, DragDropEffect&> dragOverEvent;
		EventSRWM<
			ComPtr<IDataObject>,
			DragDropModifierInfo,
			PointF, DragDropEffect&> dropEvent;
	};
}
