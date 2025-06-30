export module PGUI.UI.Layout.LayoutEnums;

export namespace PGUI::UI::Layout
{
	/// <summary>
	/// Specifies the orientation of a StackLayout
	/// </summary>
	enum class Orientation
	{
		/// <summary>
		/// Children are arranged vertically (top to bottom)
		/// </summary>
		Vertical,
		
		/// <summary>
		/// Children are arranged horizontally (left to right)
		/// </summary>
		Horizontal
	};

	/// <summary>
	/// Specifies how children are aligned within the StackLayout
	/// </summary>
	enum class Alignment
	{
		/// <summary>
		/// Align to the start (left for horizontal, top for vertical)
		/// </summary>
		Start,
		
		/// <summary>
		/// Center alignment
		/// </summary>
		Center,
		
		/// <summary>
		/// Align to the end (right for horizontal, bottom for vertical)
		/// </summary>
		End,
		
		/// <summary>
		/// Stretch to fill available space
		/// </summary>
		Stretch
	};

	/// <summary>
	/// Specifies how a child should grow or shrink within the layout
	/// </summary>
	enum class SizeToContent
	{
		/// <summary>
		/// Child maintains its natural size
		/// </summary>
		None,
		
		/// <summary>
		/// Child can grow to fill available space
		/// </summary>
		CanGrow,
		
		/// <summary>
		/// Child can shrink below its natural size
		/// </summary>
		CanShrink,
		
		/// <summary>
		/// Child can both grow and shrink
		/// </summary>
		CanGrowAndShrink
	};
}