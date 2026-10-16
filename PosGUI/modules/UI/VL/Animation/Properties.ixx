module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

export module PGUI.UI.VL.Properties;

import std;

import PGUI.Utils;
import PGUI.UI.VL.Expression;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL::Properties
{
	template <WStringLiteral Name, typename Owner, Expression::ExpressionTypes::ExpressionType T>
	struct Property
	{
		using OwnerHandleType = Owner;
		using ValueTag = T;
		//TODO restore once MSVC stops ICEing on nameLiteral across module import
		//static constexpr auto nameLiteral = Name;
		static constexpr std::wstring_view name = Name.data;
	};
	
	template <typename T>
	concept IsProperty = requires {
		{ T::name } -> std::convertible_to<std::wstring_view>;
		//requires std::same_as<std::remove_cvref_t<decltype(T::nameLiteral)>, WStringLiteral<T::name.size() + 1>>;
		typename T::OwnerHandleType;
	} && Expression::ExpressionTypes::ExpressionType<typename T::ValueTag>;
	
	//template <IsProperty P, WStringLiteral Sub,
	//	Expression::ExpressionTypes::ExpressionType T = Expression::ExpressionTypes::Scalar>
	//using SubProperty = Property<Concat(
	//			P::nameLiteral, WStringLiteral{ L"." }, Sub
	//		), typename P::OwnerHandleType, T>;

	namespace Visual
	{
		using AnchorPoint = Property<L"AnchorPoint", WUC::Visual, Expression::ExpressionTypes::Vector2>;
		using AnchorPointX = Property<L"AnchorPoint.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using AnchorPointY = Property<L"AnchorPoint.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using CenterPoint = Property<L"CenterPoint", WUC::Visual, Expression::ExpressionTypes::Vector3>;
		using CenterPointX = Property<L"CenterPoint.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using CenterPointY = Property<L"CenterPoint.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using CenterPointZ = Property<L"CenterPoint.Z", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using IsVisible = Property<L"IsVisible", WUC::Visual, Expression::ExpressionTypes::Bool>;
		using Offset = Property<L"Offset", WUC::Visual, Expression::ExpressionTypes::Vector3>;
		using OffsetX = Property<L"Offset.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using OffsetZ = Property<L"Offset.Z", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using Opacity = Property<L"Opacity", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using Orientation = Property<L"Orientation", WUC::Visual, Expression::ExpressionTypes::Quaternion>;
		using RelativeOffsetAdjustment = Property<L"RelativeOffsetAdjustment", WUC::Visual, Expression::ExpressionTypes::Vector3>;
		using RelativeOffsetAdjustmentX = Property<L"RelativeOffsetAdjustment.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RelativeOffsetAdjustmentY = Property<L"RelativeOffsetAdjustment.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RelativeOffsetAdjustmentZ = Property<L"RelativeOffsetAdjustment.Z", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RelativeSizeAdjustment = Property<L"RelativeSizeAdjustment", WUC::Visual, Expression::ExpressionTypes::Vector2>;
		using RelativeSizeAdjustmentX = Property<L"RelativeSizeAdjustment.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RelativeSizeAdjustmentY = Property<L"RelativeSizeAdjustment.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RotationAngle = Property<L"RotationAngle", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RotationAngleInDegrees = Property<L"RotationAngleInDegrees", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RotationAxis = Property<L"RotationAxis", WUC::Visual, Expression::ExpressionTypes::Vector3>;
		using RotationAxisX = Property<L"RotationAxis.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RotationAxisY = Property<L"RotationAxis.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using RotationAxisZ = Property<L"RotationAxis.Z", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using Scale = Property<L"Scale", WUC::Visual, Expression::ExpressionTypes::Vector3>;
		using ScaleX = Property<L"Scale.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using ScaleY = Property<L"Scale.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using ScaleZ = Property<L"Scale.Z", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using Size = Property<L"Size", WUC::Visual, Expression::ExpressionTypes::Vector2>;
		using SizeX = Property<L"Size.X", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using SizeY = Property<L"Size.Y", WUC::Visual, Expression::ExpressionTypes::Scalar>;
		using TransformMatrix = Property<L"TransformMatrix", WUC::Visual, Expression::ExpressionTypes::Matrix4X4>;
	}
	namespace ColorBrush
	{
		using Color = Property<L"Color", WUC::CompositionColorBrush, Expression::ExpressionTypes::Color>;
	}
	namespace Clip
	{
		using AnchorPoint = Property<L"AnchorPoint", WUC::CompositionClip, Expression::ExpressionTypes::Vector2>;
		using AnchorPointX = Property<L"AnchorPoint.X", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using AnchorPointY = Property<L"AnchorPoint.Y", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using CenterPoint = Property<L"CenterPoint", WUC::CompositionClip, Expression::ExpressionTypes::Vector2>;
		using CenterPointX = Property<L"CenterPoint.X", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using CenterPointY = Property<L"CenterPoint.Y", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using Offset = Property<L"Offset", WUC::CompositionClip, Expression::ExpressionTypes::Vector2>;
		using OffsetX = Property<L"Offset.X", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using RotationAngle = Property<L"RotationAngle", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using RotationAngleInDegrees = Property<L"RotationAngleInDegrees", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using Scale = Property<L"Scale", WUC::CompositionClip, Expression::ExpressionTypes::Vector2>;
		using ScaleX = Property<L"Scale.X", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using ScaleY = Property<L"Scale.Y", WUC::CompositionClip, Expression::ExpressionTypes::Scalar>;
		using TransformMatrix = Property<L"TransformMatrix", WUC::CompositionClip, Expression::ExpressionTypes::Matrix3X2>;
	}
	namespace InsetClip
	{
		using BottomInset = Property<L"BottomInset", WUC::InsetClip, Expression::ExpressionTypes::Scalar>;
		using LeftInset = Property<L"LeftInset", WUC::InsetClip, Expression::ExpressionTypes::Scalar>;
		using RightInset = Property<L"RightInset", WUC::InsetClip, Expression::ExpressionTypes::Scalar>;
		using TopInset = Property<L"TopInset", WUC::InsetClip, Expression::ExpressionTypes::Scalar>;
	}
	namespace RectangleClip
	{
		using Bottom = Property<L"Bottom", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using BottomLeftRadius = Property<L"BottomLeftRadius", WUC::RectangleClip, Expression::ExpressionTypes::Vector2>;
		using BottomLeftRadiusX = Property<L"BottomLeftRadius.X", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using BottomLeftRadiusY = Property<L"BottomLeftRadius.Y", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using BottomRightRadius = Property<L"BottomRightRadius", WUC::RectangleClip, Expression::ExpressionTypes::Vector2>;
		using BottomRightRadiusX = Property<L"BottomRightRadius.X", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using BottomRightRadiusY = Property<L"BottomRightRadius.Y", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using Left = Property<L"Left", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using Right = Property<L"Right", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using Top = Property<L"Top", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using TopLeftRadius = Property<L"TopLeftRadius", WUC::RectangleClip, Expression::ExpressionTypes::Vector2>;
		using TopLeftRadiusX = Property<L"TopLeftRadius.X", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using TopLeftRadiusY = Property<L"TopLeftRadius.Y", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using TopRightRadius = Property<L"TopRightRadius", WUC::RectangleClip, Expression::ExpressionTypes::Vector2>;
		using TopRightRadiusX = Property<L"TopRightRadius.X", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
		using TopRightRadiusY = Property<L"TopRightRadius.Y", WUC::RectangleClip, Expression::ExpressionTypes::Scalar>;
	}
	namespace SurfaceBrush
	{
		using AnchorPoint = Property<L"AnchorPoint", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Vector2>;
		using AnchorPointX = Property<L"AnchorPoint.X", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using AnchorPointY = Property<L"AnchorPoint.Y", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using CenterPoint = Property<L"CenterPoint", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Vector2>;
		using CenterPointX = Property<L"CenterPoint.X", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using CenterPointY = Property<L"CenterPoint.Y", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using HorizontalAlignmentRatio = Property<L"HorizontalAlignmentRatio", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using Offset = Property<L"Offset", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Vector2>;
		using OffsetX = Property<L"Offset.X", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using RotationAngle = Property<L"RotationAngle", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using RotationAngleInDegrees = Property<L"RotationAngleInDegrees", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using Scale = Property<L"Scale", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Vector2>;
		using ScaleX = Property<L"Scale.X", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using ScaleY = Property<L"Scale.Y", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
		using TransformMatrix = Property<L"TransformMatrix", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Matrix3X2>;
		using VerticalAlignmentRatio = Property<L"VerticalAlignmentRatio", WUC::CompositionSurfaceBrush, Expression::ExpressionTypes::Scalar>;
	}
	namespace Geometry
	{
		using TrimEnd = Property<L"TrimEnd", WUC::CompositionGeometry, Expression::ExpressionTypes::Scalar>;
		using TrimOffset = Property<L"TrimOffset", WUC::CompositionGeometry, Expression::ExpressionTypes::Scalar>;
		using TrimStart = Property<L"TrimStart", WUC::CompositionGeometry, Expression::ExpressionTypes::Scalar>;
	}
	namespace EllipseGeometry
	{
		using Center = Property<L"Center", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Vector2>;
		using CenterX = Property<L"Center.X", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Scalar>;
		using CenterY = Property<L"Center.Y", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Scalar>;
		using Radius = Property<L"Radius", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Vector2>;
		using RadiusX = Property<L"Radius.X", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Scalar>;
		using RadiusY = Property<L"Radius.Y", WUC::CompositionEllipseGeometry, Expression::ExpressionTypes::Scalar>;
	}
	namespace LineGeometry
	{
		using End = Property<L"End", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Vector2>;
		using EndX = Property<L"End.X", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Scalar>;
		using EndY = Property<L"End.Y", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Scalar>;
		using Start = Property<L"Start", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Vector2>;
		using StartX = Property<L"Start.X", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Scalar>;
		using StartY = Property<L"Start.Y", WUC::CompositionLineGeometry, Expression::ExpressionTypes::Scalar>;
	}
	namespace RectangleGeometry
	{
		using Offset = Property<L"Offset", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Vector2>;
		using OffsetX = Property<L"Offset.X", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using Size = Property<L"Size", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Vector2>;
		using SizeX = Property<L"Size.X", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using SizeY = Property<L"Size.Y", WUC::CompositionRectangleGeometry, Expression::ExpressionTypes::Scalar>;
	}
	namespace RoundedRectangleGeometry
	{
		using CornerRadius = Property<L"CornerRadius", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Vector2>;
		using CornerRadiusX = Property<L"CornerRadius.X", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using CornerRadiusY = Property<L"CornerRadius.Y", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using Offset = Property<L"Offset", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Vector2>;
		using OffsetX = Property<L"Offset.X", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using Size = Property<L"Size", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Vector2>;
		using SizeX = Property<L"Size.X", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
		using SizeY = Property<L"Size.Y", WUC::CompositionRoundedRectangleGeometry, Expression::ExpressionTypes::Scalar>;
	}
	namespace ViewBox
	{
		using HorizontalAlignmentRatio = Property<L"HorizontalAlignmentRatio", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
		using Offset = Property<L"Offset", WUC::CompositionViewBox, Expression::ExpressionTypes::Vector2>;
		using OffsetX = Property<L"Offset.X", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
		using OffsetY = Property<L"Offset.Y", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
		using Size = Property<L"Size", WUC::CompositionViewBox, Expression::ExpressionTypes::Vector2>;
		using SizeX = Property<L"Size.X", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
		using SizeY = Property<L"Size.Y", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
		using VerticalAlignmentRatio = Property<L"VerticalAlignmentRatio", WUC::CompositionViewBox, Expression::ExpressionTypes::Scalar>;
	}
	namespace InteractionTracker
	{
		// using IsInertiaFromImpulse = Property<L"IsInertiaFromImpulse", WUCI::InteractionTracker, Expression::ExpressionTypes::Bool>;
		// using IsPositionRoundingSuggested = Property<L"IsPositionRoundingSuggested", WUCI::InteractionTracker, Expression::ExpressionTypes::Bool>;
		using MaxPosition = Property<L"MaxPosition", WUCI::InteractionTracker, Expression::ExpressionTypes::Vector3>;
		using MaxPositionX = Property<L"MaxPosition.X", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MaxPositionY = Property<L"MaxPosition.Y", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MaxPositionZ = Property<L"MaxPosition.Z", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MaxScale = Property<L"MaxScale", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MinPosition = Property<L"MinPosition", WUCI::InteractionTracker, Expression::ExpressionTypes::Vector3>;
		using MinPositionX = Property<L"MinPosition.X", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MinPositionY = Property<L"MinPosition.Y", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MinPositionZ = Property<L"MinPosition.Z", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using MinScale = Property<L"MinScale", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using NaturalRestingPosition = Property<L"NaturalRestingPosition", WUCI::InteractionTracker, Expression::ExpressionTypes::Vector3>;
		// using NaturalRestingPositionX = Property<L"NaturalRestingPosition.X", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using NaturalRestingPositionY = Property<L"NaturalRestingPosition.Y", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using NaturalRestingPositionZ = Property<L"NaturalRestingPosition.Z", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using NaturalRestingScale = Property<L"NaturalRestingScale", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using Position = Property<L"Position", WUCI::InteractionTracker, Expression::ExpressionTypes::Vector3>;
		using PositionX = Property<L"Position.X", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using PositionY = Property<L"Position.Y", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using PositionZ = Property<L"Position.Z", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using PositionVelocityInPixelsPerSecond = Property<L"PositionVelocityInPixelsPerSecond", WUCI::InteractionTracker, Expression::ExpressionTypes::Vector3>;
		// using PositionVelocityInPixelsPerSecondX = Property<L"PositionVelocityInPixelsPerSecond.X", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using PositionVelocityInPixelsPerSecondY = Property<L"PositionVelocityInPixelsPerSecond.Y", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using PositionVelocityInPixelsPerSecondZ = Property<L"PositionVelocityInPixelsPerSecond.Z", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		using Scale = Property<L"Scale", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
		// using ScaleVelocityInPercentPerSecond = Property<L"ScaleVelocityInPercentPerSecond", WUCI::InteractionTracker, Expression::ExpressionTypes::Scalar>;
	}
}
