module;
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <wrl.h>

module PGUI.UI.Clip;

import std;

import PGUI.Window;
import PGUI.Factories;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Shape2D;
import PGUI.UI.D2D.D2DPathGeometry;

namespace PGUI::UI
{
	RoundedRectangleClip::RoundedRectangleClip(const ComPtr<ID2D1RoundedRectangleGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	RoundedRectangleClip::RoundedRectangleClip(const RoundedRect& rect) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		const auto hr = factory->CreateRoundedRectangleGeometry(rect, GetAddress());
		LogIfFailed(
			Error{
				hr
			}
			.AddTag(ErrorTags::Creation)
			.AddDetail(L"rect", std::format(L"{}", rect))
		);
	}

	RectangleClip::RectangleClip(const ComPtr<ID2D1RectangleGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	RectangleClip::RectangleClip(const RectF rect) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		const auto hr = factory->CreateRectangleGeometry(rect, GetAddress());
		LogIfFailed(
			Error{
				hr
			}
			.AddTag(ErrorTags::Creation)
			.AddDetail(L"rect", std::format(L"{}", rect))
		);
	}

	EllipseClip::EllipseClip(const ComPtr<ID2D1EllipseGeometry>& geometry) noexcept :
		ComPtrHolder{ geometry }
	{ }

	EllipseClip::EllipseClip(const Ellipse ellipse) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		const auto hr = factory->CreateEllipseGeometry(ellipse, GetAddress());
		LogIfFailed(
			Error{
				hr
			}
			.AddTag(ErrorTags::Creation)
			.AddDetail(L"ellipse", std::format(L"{}", ellipse))
		);
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

	Clip::Clip(ClipParameters parameters) noexcept :
		parameters{ parameters }
	{ }

	auto Clip::CreateClip() -> void
	{
		ReleaseClip();
		std::visit([this]<typename T>(const T& parameters)
		{
			if constexpr (std::is_same_v<T, RectangleClipParameters>)
			{
				clip = RectangleClip{ parameters.rect };
			}
			else if constexpr (std::is_same_v<T, RoundedRectangleClipParameters>)
			{
				clip = RoundedRectangleClip{ parameters.rect };
			}
			else if constexpr (std::is_same_v<T, EllipseClipParameters>)
			{
				clip = EllipseClip{ parameters.ellipse };
			}
			else if constexpr (std::is_same_v<T, PathClipParameters>)
			{
				clip = PathClip{ parameters.path };
			}
			else if constexpr (std::is_same_v<T, RoundCornerClipParameters>)
			{
				auto pathResult = D2D::D2DPathGeometry::CreateRoundRectWithPathGeometry(
					parameters.rect, parameters.topLeftRadius,
					parameters.topRightRadius, parameters.bottomLeftRadius,
					parameters.bottomRightRadius);
				if (pathResult.has_value())
				{
					clip = RoundCornerClip{ pathResult.value() };
				}
			}
		}, parameters);
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
	// ReSharper disable once CppInconsistentNaming
	auto Clip::SetParameters(const ClipParameters& _parameters) noexcept -> void
	{
		if (!std::holds_alternative<RoundedRectangleClipParameters>(_parameters) &&
		    !std::holds_alternative<RoundCornerClipParameters>(_parameters))
		{
			parameters = _parameters;
			return;
		}
		if (std::holds_alternative<RoundedRectangleClipParameters>(_parameters))
		{
			if (const auto& params = std::get<RoundedRectangleClipParameters>(_parameters);
				params.rect.xRadius == 0.0f && params.rect.yRadius == 0.0f)
			{
				parameters = RectangleClipParameters{ RectF{ params.rect } };
				return;
			}
		}

		const auto& params = std::get<RoundCornerClipParameters>(_parameters);
		if (params.topLeftRadius != params.topRightRadius ||
		    params.topRightRadius != params.bottomLeftRadius ||
		    params.bottomLeftRadius != params.bottomRightRadius)
		{
			parameters = _parameters;
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
		return std::visit([]<typename T>(const T& clip) -> D2D::D2DGeometry<>
		{
			if constexpr (!std::is_same_v<EmptyClip, T>)
			{
				return D2D::D2DGeometry<>{ clip.Get() };
			}
			else
			{
				return D2D::D2DGeometry<>{ nullptr };
			}
		}, clip);
	}

	Clip::operator ID2D1Geometry*() const noexcept
	{
		return GetGeometry().GetRaw();
	}
}
