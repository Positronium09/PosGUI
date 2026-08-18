module;
#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Quaternion;

namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI
{
	struct Quaternion
	{
		float x{ 0.0F };
		float y{ 0.0F };
		float z{ 0.0F };
		float w{ 1.0F };

		constexpr Quaternion() noexcept = default;
		constexpr Quaternion(
			const float x, const float y, 
			const float z, const float w) noexcept :
			x{ x }, y{ y }, z{ z }, w{ w }
		{ }

		explicit(false) constexpr Quaternion(const WFN::quaternion& quaternion) noexcept :
			x{ quaternion.x }, y{ quaternion.y }, z{ quaternion.z }, w{ quaternion.w }
		{ }

		explicit(false) constexpr operator WFN::quaternion() const noexcept
		{
			return WFN::quaternion{ x, y, z, w };
		}
	};
}