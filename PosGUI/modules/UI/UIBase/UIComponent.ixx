export module PGUI.UI.UIBase:UIComponent;

import :UIElement;
import PGUI.Shape2D;
import PGUI.UI.Clip;
import PGUI.UI.Graphics;

export namespace PGUI::UI
{
	class UIComponent : public UIElement
	{
		public:
		auto Render(Graphics graphics) -> void override;

		auto Move(PointF position) -> void override;
		auto Resize(SizeF size) -> void override;
		auto MoveAndResize(RectF newRect) -> void override;
		auto MoveAndResize(const PointF position, const SizeF size) -> void override
		{
			MoveAndResize(RectF{ position, size });
		}

		[[nodiscard]] auto GetRect() const noexcept -> RectF override
		{
			return rect;
		}
		[[nodiscard]] auto GetPosition() const noexcept -> PointF override
		{
			return rect.TopLeft();
		}
		[[nodiscard]] auto GetSize() const noexcept -> SizeF override
		{
			return rect.Size();
		}

		[[nodiscard]] auto HitTest(PointF point) noexcept -> bool override;

		[[nodiscard]] const auto& GetClip() const noexcept
		{
			return clip;
		}
		auto SetClip(Clip newClip) noexcept -> void
		{
			clip = std::move(newClip);
			if (!clip)
			{
				clip.CreateClip();
			}
			Invalidate();
		}

		protected:
		virtual auto ClippedRender(Graphics) -> void
		{
			/*  */
		}

		private:
		RectF rect;
		Clip clip;
	};
}
