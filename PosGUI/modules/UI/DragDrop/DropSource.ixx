module;
#include <ole2.h>

export module PGUI.UI.DragDrop.DropSource;

import PGUI.Event;
import PGUI.ComPtr;
import PGUI.UI.DragDrop.DragDropTypes;

export namespace PGUI::UI::DragDrop
{
	class DropSource : public Implements<DropSource, IDropSource>
	{
		public:
		DropSource() = default;
		~DropSource() noexcept override = default;

		virtual auto OnQueryContinueDrag(bool escapePressed, DragDropModifierInfo modifierInfo) -> void = 0;
		virtual auto OnGiveFeedback(DragDropEffect effect) -> void = 0;

		private:
		auto QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) -> HRESULT override;
		auto GiveFeedback(DWORD dwEffect) -> HRESULT override;
	};

	class DropSourceEvent final : public DropSource
	{
		public:
		DropSourceEvent() = default;
		
		[[nodiscard]] auto& QueryContinueDragEvent() noexcept { return queryContinueDragEvent; }
		[[nodiscard]] const auto& QueryContinueDragEvent() const noexcept { return queryContinueDragEvent; }
		[[nodiscard]] auto& GiveFeedbackEvent() noexcept { return giveFeedbackEvent; }
		[[nodiscard]] const auto& GiveFeedbackEvent() const noexcept { return giveFeedbackEvent; }

		auto OnQueryContinueDrag(const bool escapePressed,
								 const DragDropModifierInfo modifierInfo) -> void override
		{
			queryContinueDragEvent.Invoke(escapePressed, modifierInfo);
		}
		auto OnGiveFeedback(const DragDropEffect effect) -> void override
		{
			giveFeedbackEvent.Invoke(effect);
		}

		private:
		Event<bool, DragDropModifierInfo> queryContinueDragEvent;
		Event<DragDropEffect> giveFeedbackEvent;
	};
}
