module;
#include <string>
#include <string_view>
#include <Windows.h>

export module PGUI.UI.AppWindow;

import PGUI.Shape2D;
import PGUI.Window;
import PGUI.UI.Color;
import PGUI.UI.Brush;
import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.ThemeAware;
import PGUI.UI.Animation.AnimationManager;
import PGUI.UI.DirectXCompositionWindow;


export namespace PGUI::UI
{
	struct AutoStopAnimations : public MessageHooker
	{
		AutoStopAnimations() noexcept
		{
			RegisterHandler(WM_SETFOCUS, [this](UINT, WPARAM, LPARAM) -> MessageHandlerResult
			{
				Animation::AnimationManager::Resume();
				GetHookedWindow()->Invalidate();

				return 0;
			});
			RegisterHandler(WM_KILLFOCUS, [](UINT, WPARAM, LPARAM) -> MessageHandlerResult
			{
				Animation::AnimationManager::Pause();
				return 0;
			});
		}
	};

	class AppWindow : public DirectXCompositionWindow, public Theming::ThemeAware<Theming::AppWindowStyle>
	{
		public:
		AppWindow() noexcept;
		AppWindow(const WindowClassPtr& wndClass) noexcept;

		[[nodiscard]] auto IsFullScreen() const noexcept { return isFullScreen; }
		void EnterFullScreenMode() noexcept;
		void ExitFullScreenMode() noexcept;

		[[nodiscard]] auto GetMinimumSize() const noexcept -> SizeI { return minSize; }
		void SetMinimumSize(SizeI size) noexcept { minSize = size; }

		void SetTitle(std::wstring_view title) const noexcept;
		[[nodiscard]] auto GetTitle() const noexcept -> std::wstring_view { return titleText; }

		[[nodiscard]] auto IsMaximizable() const noexcept -> bool;
		void SetMaximizable(bool isMaximizable) const noexcept;

		[[nodiscard]] auto IsMinimizable() const noexcept -> bool;
		void SetMinimizable(bool isMinimizable) const noexcept;

		[[nodiscard]] auto IsCloseable() const noexcept -> bool;
		void SetCloseable(bool isCloseable) const noexcept;

		[[nodiscard]] auto IsAlwaysOnTop() const noexcept -> bool;
		void SetAlwaysOnTop(bool isAlwaysOnTop) const noexcept;

		[[nodiscard]] auto IsResizable() const noexcept -> bool;
		void SetResizable(bool isResizable) const noexcept;

		[[nodiscard]] auto IsAutoStopAnimationsEnabled() const noexcept { return autoStopAnimations; }
		void SetAutoStopAnimations(bool enable) noexcept;

		void SetBorderColor(RGBA color) const noexcept;
		void SetCaptionColor(RGBA color) const noexcept;
		void SetCaptionTextColor(RGBA color) const noexcept;
		void SetCornerPreference(Theming::CornerPreference cornerPreference) const noexcept;

		void ApplyStyle(const Theming::AppWindowStyle& style) noexcept override;

		private:
		std::wstring titleText;
		WindowPlacement prevPlacement{ };
		bool isFullScreen = false;
		bool autoStopAnimations = true;
		AutoStopAnimations autoStop;
		SizeI minSize = SizeI{ 300, 300 };

		auto OnNCCreate(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;
		auto OnSetText(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;
		[[nodiscard]] auto OnGetText(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
		[[nodiscard]] auto OnGetTextLength(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
		[[nodiscard]] auto OnGetMinMaxInfo(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
		[[nodiscard]] auto OnLButtonDown(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
	};
}
