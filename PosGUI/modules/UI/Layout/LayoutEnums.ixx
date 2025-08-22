export module PGUI.UI.Layout.LayoutEnums;

export namespace PGUI::UI::Layout
{
	enum class LayoutOrientation
	{
		Horizontal,
		Vertical
	};

	enum class MainAxisAlignment
	{
		Start,
		Center,
		End,
		SpaceBetween,
		SpaceAround,
		SpaceEvenly
	};

	enum class CrossAxisAlignment
	{
		Start,
		Center,
		End,
		Stretch
	};

	enum class WrapMode
	{
		NoWrap,
		Wrap,
	};
}
