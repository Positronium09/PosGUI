module;
#include <ole2.h>

export module PGUI.UI.DragDrop;

import PGUI.Event;
import PGUI.ComPtr;
import PGUI.UI.Input;

export namespace PGUI::UI
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

	class DragDrop : Implements<DragDrop, IDropTarget>
	{
		public:
		DragDrop() = default;
		~DragDrop() noexcept override = default;

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

	class DragDropEvent final : public DragDrop
	{
		public:
		DragDropEvent() = default;

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
