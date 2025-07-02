module;
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <wrl.h>

module PGUI.UI.Clip;

import std;

import PGUI.Window;
import PGUI.Factories;
import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.D2D.D2DPathGeometry;

namespace PGUI::UI
{
	RoundedRectangleClip::RoundedRectangleClip(ComPtr<ID2D1RoundedRectangleGeometry> geometry) noexcept : 
		ComPtrHolder{ geometry }
	{
	}
	RoundedRectangleClip::RoundedRectangleClip(RoundedRect rect) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		factory->CreateRoundedRectangleGeometry(rect, GetAddress());
	}
	
	RectangleClip::RectangleClip(ComPtr<ID2D1RectangleGeometry> geometry) noexcept : 
		ComPtrHolder{ geometry }
	{
	}
	RectangleClip::RectangleClip(RectF rect) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();
	
		factory->CreateRectangleGeometry(rect, GetAddress());
	}

	EllipseClip::EllipseClip(ComPtr<ID2D1EllipseGeometry> geometry) noexcept : 
		ComPtrHolder{ geometry }
	{
	}
	EllipseClip::EllipseClip(Ellipse ellipse) noexcept
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		factory->CreateEllipseGeometry(ellipse, GetAddress());
	}


	PathClip::PathClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept : 
		ComPtrHolder{ geometry }
	{
	}
	PathClip::PathClip(const D2D::D2DPathGeometry& geometry) noexcept : 
		PathClip{ geometry.Get() }
	{
	}


	RoundCornerClip::RoundCornerClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept : 
		PathClip{ geometry }
	{
	}
	RoundCornerClip::RoundCornerClip(const D2D::D2DPathGeometry& geometry) noexcept : 
		PathClip{ geometry }
	{
	}

	Clip::Clip(ClipParameters parameters) noexcept : 
		parameters{ parameters }
	{
	}
	void Clip::CreateClip()
	{
		ReleaseClip();
		std::visit([this]<typename T>(const T & parameters)
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
				auto path = D2D::D2DPathGeometry::CreateRoundRectWithPathGeometry(
					parameters.rect, parameters.topLeftRadius,
					parameters.topRightRadius, parameters.bottomLeftRadius,
					parameters.bottomRightRadius);
				clip = RoundCornerClip{ path };
			}
		}, parameters);
	}
	void Clip::ReleaseClip() noexcept
	{
		clip = EmptyClip{ };
	}
	void Clip::Clear() noexcept
	{
		ReleaseClip();
		parameters = EmptyClipParameters{ };
	}

	void Clip::SetParameters(ClipParameters _parameters) noexcept
	{
		if (!std::holds_alternative<RoundedRectangleClipParameters>(_parameters) &&
			!std::holds_alternative<RoundCornerClipParameters>(_parameters))
		{
			parameters = _parameters;
			return;
		}
		else if (std::holds_alternative<RoundedRectangleClipParameters>(_parameters))
		{
			const auto& params = std::get<RoundedRectangleClipParameters>(_parameters);
			if (params.rect.xRadius == 0.0f && params.rect.yRadius == 0.0f)
			{
				parameters = RectangleClipParameters{ params.rect };
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

		parameters = RoundedRectangleClipParameters{ RoundedRect{ params.rect, params.topLeftRadius, params.topLeftRadius } };
	}

	auto Clip::GetGeometry() const noexcept -> D2D::D2DGeometry<>
	{
		return std::visit([]<typename T>(const T & clip) -> D2D::D2DGeometry<>
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
