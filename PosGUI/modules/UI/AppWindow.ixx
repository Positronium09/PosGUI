module;
#include <Windows.h>

export module PGUI.UI.AppWindow;

import std;

import PGUI.Shape2D;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.Color;
import PGUI.UI.Brush;
import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.ThemeAware;
import PGUI.UI.Animation;
import PGUI.UI.UIBase;

export namespace PGUI::UI
{
	class AppWindow : public UIWindow , public Theming::ThemeAware<Theming::AppWindowStyle>
	{
		public:
		AppWindow() noexcept;

		explicit AppWindow(const WindowClassPtr& wndClass) noexcept;

		[[nodiscard]] auto IsFullScreen() const noexcept { return isFullScreen; }

		auto EnterFullScreenMode() noexcept -> void;

		auto ExitFullScreenMode() noexcept -> void;

		[[nodiscard]] auto GetMinimumSize() const noexcept -> SizeI { return minSize; }
		auto SetMinimumSize(const SizeI size) noexcept -> void { minSize = size; }

		auto SetTitle(std::wstring_view title) const noexcept -> void;

		[[nodiscard]] auto GetTitle() const noexcept -> std::wstring_view { return titleText; }

		[[nodiscard]] auto IsMaximizable() const noexcept -> bool;

		auto SetMaximizable(bool isMaximizable) const noexcept -> void;

		[[nodiscard]] auto IsMinimizable() const noexcept -> bool;

		auto SetMinimizable(bool isMinimizable) const noexcept -> void;

		[[nodiscard]] auto IsCloseable() const noexcept -> bool;

		auto SetCloseable(bool isCloseable) const noexcept -> void;

		[[nodiscard]] auto IsAlwaysOnTop() const noexcept -> bool;

		auto SetAlwaysOnTop(bool isAlwaysOnTop) const noexcept -> void;

		[[nodiscard]] auto IsResizable() const noexcept -> bool;

		auto SetResizable(bool isResizable) const noexcept -> void;

		auto SetBorderColor(RGBA color) const noexcept -> void;

		auto SetCaptionColor(RGBA color) const noexcept -> void;

		auto SetCaptionTextColor(RGBA color) const noexcept -> void;

		auto SetCornerPreference(Theming::CornerPreference cornerPreference) const noexcept -> void;

		auto ApplyStyle(const Theming::AppWindowStyle& style) noexcept -> void override;

		private:
		std::wstring titleText;
		WindowPlacement prevPlacement{ };
		bool isFullScreen = false;
		SizeI minSize = SizeI{ 300, 300 };

		auto OnNCCreate(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;

		auto OnSetText(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;

		[[nodiscard]] auto OnGetText(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;

		[[nodiscard]] auto OnGetTextLength(UINT msg, WPARAM wParam,
		                                   LPARAM lParam) const noexcept -> MessageHandlerResult;

		[[nodiscard]] auto OnGetMinMaxInfo(UINT msg, WPARAM wParam,
		                                   LPARAM lParam) const noexcept -> MessageHandlerResult;

		[[nodiscard]] auto OnLButtonDown(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
	};
}
