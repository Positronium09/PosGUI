module;
#include <unordered_map>
#include <optional>
#include <vector>
#include <Windows.h>

export module PGUI.UI.Layout.StackLayout;

import PGUI.Shape2D;
import PGUI.UI.Layout.LayoutEnums;

export namespace PGUI::UI::Layout
{
	/// <summary>
	/// Layout constraints for a single child element
	/// </summary>
	struct LayoutConstraints
	{
		/// <summary>
		/// Minimum size constraint
		/// </summary>
		std::optional<PGUI::SizeF> minSize;
		
		/// <summary>
		/// Maximum size constraint  
		/// </summary>
		std::optional<PGUI::SizeF> maxSize;
		
		/// <summary>
		/// Margin around the child
		/// </summary>
		PGUI::RectF margin{ 0.0f, 0.0f, 0.0f, 0.0f };
		
		/// <summary>
		/// How the child should behave with respect to growing/shrinking
		/// </summary>
		SizeToContent sizeToContent = SizeToContent::None;
		
		/// <summary>
		/// Alignment override for this specific child (if different from container default)
		/// </summary>
		std::optional<Alignment> alignmentOverride;

		constexpr LayoutConstraints() noexcept = default;
		
		constexpr LayoutConstraints(
			std::optional<PGUI::SizeF> minSize_,
			std::optional<PGUI::SizeF> maxSize_ = std::nullopt,
			PGUI::RectF margin_ = {},
			SizeToContent sizeToContent_ = SizeToContent::None,
			std::optional<Alignment> alignmentOverride_ = std::nullopt
		) noexcept :
			minSize{ minSize_ },
			maxSize{ maxSize_ },
			margin{ margin_ },
			sizeToContent{ sizeToContent_ },
			alignmentOverride{ alignmentOverride_ }
		{
		}
	};

	/// <summary>
	/// Result of the measure phase for a child
	/// </summary>
	struct MeasureResult
	{
		/// <summary>
		/// Desired size of the child including margin
		/// </summary>
		PGUI::SizeF desiredSize;
		
		/// <summary>
		/// Minimum size the child can be arranged to
		/// </summary>
		PGUI::SizeF minSize;
		
		/// <summary>
		/// Maximum size the child can be arranged to
		/// </summary>
		PGUI::SizeF maxSize;

		constexpr MeasureResult() noexcept = default;
		
		constexpr MeasureResult(
			PGUI::SizeF desiredSize_,
			PGUI::SizeF minSize_,
			PGUI::SizeF maxSize_
		) noexcept :
			desiredSize{ desiredSize_ },
			minSize{ minSize_ },
			maxSize{ maxSize_ }
		{
		}
	};

	/// <summary>
	/// Core StackLayout implementation providing two-phase layout algorithm
	/// </summary>
	class StackLayout
	{
	public:
		constexpr StackLayout() noexcept = default;
		explicit constexpr StackLayout(Orientation orientation_) noexcept : 
			orientation{ orientation_ } {}

		/// <summary>
		/// Gets the orientation of the stack layout
		/// </summary>
		[[nodiscard]] constexpr auto GetOrientation() const noexcept -> Orientation 
		{ 
			return orientation; 
		}
		
		/// <summary>
		/// Sets the orientation of the stack layout
		/// </summary>
		constexpr void SetOrientation(Orientation orientation_) noexcept 
		{ 
			orientation = orientation_; 
		}

		/// <summary>
		/// Gets the default alignment for children
		/// </summary>
		[[nodiscard]] constexpr auto GetAlignment() const noexcept -> Alignment 
		{ 
			return alignment; 
		}
		
		/// <summary>
		/// Sets the default alignment for children
		/// </summary>
		constexpr void SetAlignment(Alignment alignment_) noexcept 
		{ 
			alignment = alignment_; 
		}

		/// <summary>
		/// Gets the spacing between children
		/// </summary>
		[[nodiscard]] constexpr auto GetSpacing() const noexcept -> float 
		{ 
			return spacing; 
		}
		
		/// <summary>
		/// Sets the spacing between children
		/// </summary>
		constexpr void SetSpacing(float spacing_) noexcept 
		{ 
			spacing = spacing_; 
		}

		/// <summary>
		/// Gets the padding of the container
		/// </summary>
		[[nodiscard]] constexpr auto GetPadding() const noexcept -> PGUI::RectF 
		{ 
			return padding; 
		}
		
		/// <summary>
		/// Sets the padding of the container
		/// </summary>
		constexpr void SetPadding(PGUI::RectF padding_) noexcept 
		{ 
			padding = padding_; 
		}

		/// <summary>
		/// Sets layout constraints for a specific child window
		/// </summary>
		void SetChildConstraints(HWND childHwnd, const LayoutConstraints& constraints);
		
		/// <summary>
		/// Gets layout constraints for a specific child window
		/// </summary>
		[[nodiscard]] auto GetChildConstraints(HWND childHwnd) const -> LayoutConstraints;
		
		/// <summary>
		/// Removes layout constraints for a specific child window
		/// </summary>
		void RemoveChildConstraints(HWND childHwnd);

		/// <summary>
		/// Performs measure phase - calculates desired sizes for all children
		/// </summary>
		[[nodiscard]] auto Measure(
			const std::vector<HWND>& children,
			PGUI::SizeF availableSize,
			float dpi = 96.0f
		) const -> std::unordered_map<HWND, MeasureResult>;

		/// <summary>
		/// Performs arrange phase - positions and sizes all children
		/// </summary>
		void Arrange(
			const std::vector<HWND>& children,
			PGUI::RectF finalRect,
			float dpi = 96.0f
		) const;

		/// <summary>
		/// Performs complete layout - measure and arrange in one call
		/// </summary>
		void Layout(
			const std::vector<HWND>& children,
			PGUI::RectF finalRect,
			float dpi = 96.0f
		) const;

	private:
		/// <summary>
		/// Gets the natural size of a child window
		/// </summary>
		[[nodiscard]] auto GetChildNaturalSize(HWND childHwnd, float dpi) const -> PGUI::SizeF;
		
		/// <summary>
		/// Applies constraints to a size
		/// </summary>
		[[nodiscard]] constexpr auto ApplyConstraints(
			PGUI::SizeF size, 
			const LayoutConstraints& constraints
		) const noexcept -> PGUI::SizeF;

		/// <summary>
		/// Calculates aligned position within available space
		/// </summary>
		[[nodiscard]] constexpr auto CalculateAlignedPosition(
			float childSize,
			float availableSize,
			Alignment alignment
		) const noexcept -> float;

		Orientation orientation = Orientation::Vertical;
		Alignment alignment = Alignment::Start;
		float spacing = 0.0f;
		PGUI::RectF padding{ 0.0f, 0.0f, 0.0f, 0.0f };
		
		std::unordered_map<HWND, LayoutConstraints> childConstraints;
	};
}