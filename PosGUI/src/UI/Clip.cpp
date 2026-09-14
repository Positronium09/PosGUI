module;
#include <d2d1_1.h>
#include <d2d1_3.h>

module PGUI.UI.Clip;

import std;

import PGUI.Window;
import PGUI.Factories;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.UI.D2D.D2DPathGeometry;

namespace PGUI::UI
{
	RectangleClip::RectangleClip(const ComPtr<ID2D1RectangleGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	auto RectangleClip::Create(const RectF& rect) noexcept -> Result<RectangleClip>
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		ComPtr<ID2D1RectangleGeometry> geometry;
		if (auto error = Error{ factory->CreateRectangleGeometry(rect, geometry.put()) };
			error.IsFailure())
		{
			error.AddDetail(L"rect", std::format(L"{}", rect));
			return Unexpected{ error };
		}

		return RectangleClip{ geometry };
	}

	RoundedRectangleClip::RoundedRectangleClip(const ComPtr<ID2D1RoundedRectangleGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	auto RoundedRectangleClip::Create(const RoundedRect& rect) noexcept -> Result<RoundedRectangleClip>
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		ComPtr<ID2D1RoundedRectangleGeometry> geometry;
		if (auto error = Error{ factory->CreateRoundedRectangleGeometry(rect, geometry.put()) };
			error.IsFailure())
		{
			error.AddDetail(L"rect", std::format(L"{}", rect));
			return Unexpected{ error };
		}

		return RoundedRectangleClip{ geometry };
	}

	EllipseClip::EllipseClip(const ComPtr<ID2D1EllipseGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	auto EllipseClip::Create(const Ellipse ellipse) noexcept -> Result<EllipseClip>
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		ComPtr<ID2D1EllipseGeometry> geometry;
		if (auto error = Error{ factory->CreateEllipseGeometry(ellipse, geometry.put()) };
			error.IsFailure())
		{
			error.AddDetail(L"ellipse", std::format(L"{}", ellipse));
			return Unexpected{ error };
		}

		return EllipseClip{ geometry };
	}


	PathClip::PathClip(const ComPtr<ID2D1PathGeometry1>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	PathClip::PathClip(const D2D::D2DPathGeometry& geometry) noexcept :
		PathClip{ geometry.Get() }
	{ }


	RoundCornerClip::RoundCornerClip(const ComPtr<ID2D1PathGeometry1>& geometry) noexcept :
		PathClip{ geometry }
	{ }

	RoundCornerClip::RoundCornerClip(const D2D::D2DPathGeometry& geometry) noexcept :
		PathClip{ geometry }
	{ }

	Clip::Clip(const ClipParameters& parameters) noexcept :
		parameters{ parameters }
	{ }

	auto Clip::CreateClip() noexcept -> Result<void>
	{
		ReleaseClip();
		return Match(parameters, [this]<typename T>(const T& parameters) -> Result<void>
		{
			const auto assign = [this](auto&& clipResult) -> Result<void>
			{
				if (!clipResult.has_value())
				{
					return Unexpected{ clipResult.error() };
				}
				clip = MoveChecked(clipResult.value());
				return EmptyResult;
			};

			if constexpr (std::is_same_v<T, RectangleClipParameters>)
			{
				return assign(RectangleClip::Create(parameters.rect));
			}
			else if constexpr (std::is_same_v<T, RoundedRectangleClipParameters>)
			{
				return assign(RoundedRectangleClip::Create(parameters.rect));
			}
			else if constexpr (std::is_same_v<T, EllipseClipParameters>)
			{
				return assign(EllipseClip::Create(parameters.ellipse));
			}
			else if constexpr (std::is_same_v<T, PathClipParameters>)
			{
				clip = PathClip{ parameters.path };
				return EmptyResult;
			}
			else if constexpr (std::is_same_v<T, RoundCornerClipParameters>)
			{
				const auto pathResult = D2D::D2DPathGeometry::CreateRoundRectWithPathGeometry(
					parameters.rect, parameters.topLeftRadius,
					parameters.topRightRadius, parameters.bottomLeftRadius,
					parameters.bottomRightRadius);
				if (!pathResult.has_value())
				{
					return Unexpected{ pathResult.error() };
				}

				clip = RoundCornerClip{ pathResult.value() };
				return EmptyResult;
			}
			else
			{
				return EmptyResult;
			}
		});
	}

	auto Clip::ReleaseClip() noexcept -> void
	{
		clip = EmptyClip{ };
	}

	auto Clip::Clear() noexcept -> void
	{
		ReleaseClip();
		parameters = EmptyClipParameters{ };
	}

	// ReSharper disable once CppParameterNamesMismatch
	auto Clip::SetParameters(const ClipParameters& newParameters) noexcept -> void
	{
		if (!std::holds_alternative<RoundedRectangleClipParameters>(newParameters) &&
		    !std::holds_alternative<RoundCornerClipParameters>(newParameters))
		{
			parameters = newParameters;
			return;
		}
		if (std::holds_alternative<RoundedRectangleClipParameters>(newParameters))
		{
			if (const auto& params = std::get<RoundedRectangleClipParameters>(newParameters);
				params.rect.xRadius == 0.0f && params.rect.yRadius == 0.0f)
			{
				parameters = RectangleClipParameters{ RectF{ params.rect } };
				return;
			}
			parameters = newParameters;
			return;
		}

		const auto& params = std::get<RoundCornerClipParameters>(newParameters);
		if (params.topLeftRadius != params.topRightRadius ||
		    params.topRightRadius != params.bottomLeftRadius ||
		    params.bottomLeftRadius != params.bottomRightRadius)
		{
			parameters = newParameters;
			return;
		}

		if (params.topLeftRadius == 0.0f)
		{
			parameters = RectangleClipParameters{ params.rect };
			return;
		}

		parameters = RoundedRectangleClipParameters{
			RoundedRect{ params.rect, params.topLeftRadius, params.topLeftRadius }
		};
	}

	auto Clip::GetGeometry() const noexcept -> D2D::D2DGeometry<>
	{
		return Match(clip, []<typename T>(const T& clip) -> D2D::D2DGeometry<>
		{
			if constexpr (!std::is_same_v<EmptyClip, T>)
			{
				return D2D::D2DGeometry<>{ clip.Get() };
			}
			else
			{
				return D2D::D2DGeometry<>{ nullptr };
			}
		});
	}

	Clip::operator ID2D1Geometry*() const noexcept
	{
		return GetGeometry().GetRaw();
	}
}
