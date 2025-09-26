export module PGUI.UI.UIBase:UIWindow;

import std;

import :UIElement;
import :UIContainer;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	class UIWindow : public DirectXCompositionWindow
	{
		public:
		UIWindow();
		explicit UIWindow(const WindowClassPtr&);

		[[nodiscard]] auto& GetChildren() noexcept
		{
			return childrenContainer;
		}
		[[nodiscard]] const auto& GetChildren() const noexcept
		{
			return childrenContainer;
		}

		protected:
		virtual auto Render(Graphics) -> void
		{
			/*  */
		}

		private:
		UIContainer childrenContainer;

		auto Draw(Graphics graphics) -> void override;
	};
}
