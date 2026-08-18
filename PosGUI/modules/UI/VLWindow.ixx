export module PGUI.UI.VLWindow;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.DesktopWindowTarget;

export namespace PGUI::UI
{
	class VLWindow : public Window
	{
		public:
		explicit VLWindow(const WindowClassPtr& wndClass) noexcept;

		private:
		VL::DesktopWindowTarget desktopWindowTarget;

		auto OnCreate(MessageID, Argument1, Argument2) noexcept -> MessageHandlerResult;
	};
}
