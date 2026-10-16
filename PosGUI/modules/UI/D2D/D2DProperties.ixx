module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DProperties;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;

export namespace PGUI::UI::D2D
{
	enum class PropertyType
	{
		Unknown = D2D1_PROPERTY_TYPE_UNKNOWN,
		String = D2D1_PROPERTY_TYPE_STRING,
		Bool = D2D1_PROPERTY_TYPE_BOOL,
		UInt32 = D2D1_PROPERTY_TYPE_UINT32,
		Int32 = D2D1_PROPERTY_TYPE_INT32,
		Float = D2D1_PROPERTY_TYPE_FLOAT,
		Vector2 = D2D1_PROPERTY_TYPE_VECTOR2,
		Vector3 = D2D1_PROPERTY_TYPE_VECTOR3,
		Vector4 = D2D1_PROPERTY_TYPE_VECTOR4,
		Blob = D2D1_PROPERTY_TYPE_BLOB,
		IUnknown = D2D1_PROPERTY_TYPE_IUNKNOWN,
		Enum = D2D1_PROPERTY_TYPE_ENUM,
		Array = D2D1_PROPERTY_TYPE_ARRAY,
		CLSID = D2D1_PROPERTY_TYPE_CLSID,
		// ReSharper disable CppInconsistentNaming
		
		Matrix3x2 = D2D1_PROPERTY_TYPE_MATRIX_3X2,
		Matrix4x3 = D2D1_PROPERTY_TYPE_MATRIX_4X3,
		Matrix4x4 = D2D1_PROPERTY_TYPE_MATRIX_4X4,
		Matrix5x4 = D2D1_PROPERTY_TYPE_MATRIX_5X4,

		// ReSharper restore CppInconsistentNaming
		
		ColorContext = D2D1_PROPERTY_TYPE_COLOR_CONTEXT,
	};

	class D2DProperties : public ComPtrHolder<ID2D1Properties>
	{
		public:
		D2DProperties() noexcept = default;

		explicit(false) D2DProperties(const ComPtr<ID2D1Properties>& properties) noexcept;

		[[nodiscard]] auto GetPropertyCount() const noexcept { return Get()->GetPropertyCount(); }

		[[nodiscard]] auto GetPropertyIndex(const wzstring_view name) const noexcept
		{
			return Get()->GetPropertyIndex(name.data());
		}

		[[nodiscard]] auto GetPropertyType(const UINT32 index) const noexcept
		{
			return FromUnderlying<PropertyType>(Get()->GetType(index));
		}

		[[nodiscard]] auto GetValueSize(const UINT32 index) const noexcept { return Get()->GetValueSize(index); }

		[[nodiscard]] auto GetPropertyName(UINT32 index) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetSubProperty(UINT32 index) const noexcept -> Result<D2DProperties>;

		template <std::size_t Extent = std::dynamic_extent>
		[[nodiscard]] auto GetProperty(const UINT32 index, std::span<BYTE, Extent> bytes) const noexcept -> Result<void>
		{
			if constexpr (Extent == std::dynamic_extent)
			{
				const auto size = GetValueSize(index);
				if (size > bytes.size())
				{
					Error error{ ErrorCode::InvalidArgument };
					error
						.AddDetail(L"index", std::to_wstring(index))
						.AddDetail(L"spanSize", std::to_wstring(bytes.size()))
						.AddDetail(L"valueSize", std::to_wstring(size));
					return Unexpected{ error };
				}

				if (const auto hr = Get()->GetValue(index, bytes.data(), size);
					FAILED(hr))
				{
					Error error{ hr };
					error
						.AddDetail(L"index", std::to_wstring(index))
						.AddDetail(L"spanSize", std::to_wstring(bytes.size()))
						.AddDetail(L"valueSize", std::to_wstring(size));
					return Unexpected{ error };
				}

				return EmptyResult;
			}
			else
			{
				if (const auto hr = Get()->GetValue(index, bytes.data(), static_cast<UINT32>(bytes.size()));
					FAILED(hr))
				{
					Error error{ hr };
					error
						.AddDetail(L"index", std::to_wstring(index))
						.AddDetail(L"spanSize", std::to_wstring(bytes.size()))
						.AddDetail(L"valueSize", std::to_wstring(GetValueSize(index)));
					return Unexpected{ error };
				}

				return EmptyResult;
			}
		}
	};
}
