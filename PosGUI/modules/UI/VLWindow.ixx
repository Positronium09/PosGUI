export module PGUI.UI.VLWindow;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.DesktopWindowTarget;
import PGUI.UI.VL.Visual;

export namespace PGUI::UI
{
	class VLWindow : public Window
	{
		public:
		explicit VLWindow(const WindowClassPtr& wndClass) noexcept;

		protected:
		auto SetRoot(const VL::Visual& visual) const noexcept -> Result<void>;

		private:
		VL::DesktopWindowTarget desktopWindowTarget;

		auto OnCreate(MessageID, Argument1, Argument2) noexcept -> MessageHandlerResult;
	};
}
