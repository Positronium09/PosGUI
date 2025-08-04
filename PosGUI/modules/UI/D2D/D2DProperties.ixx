module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DProperties;

import std;

import PGUI.ComPtr;
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
		Matrix3x2 = D2D1_PROPERTY_TYPE_MATRIX_3X2,
		Matrix4x3 = D2D1_PROPERTY_TYPE_MATRIX_4X3,
		Matrix4x4 = D2D1_PROPERTY_TYPE_MATRIX_4X4,
		Matrix5x4 = D2D1_PROPERTY_TYPE_MATRIX_5X4,
		ColorContext = D2D1_PROPERTY_TYPE_COLOR_CONTEXT,
	};

	class D2DProperties : public ComPtrHolder<ID2D1Properties>
	{
		public:
		D2DProperties() noexcept = default;

		explicit(false) D2DProperties(const ComPtr<ID2D1Properties>& properties) noexcept;

		[[nodiscard]] auto GetPropertyCount() const noexcept { return Get()->GetPropertyCount(); }

		[[nodiscard]] auto GetPropertyIndex(const std::wstring_view name) const noexcept
		{
			return Get()->GetPropertyIndex(name.data());
		}

		[[nodiscard]] auto GetPropertyType(const UINT32 index) const noexcept
		{
			return static_cast<PropertyType>(Get()->GetType(index));
		}

		[[nodiscard]] auto GetValueSize(const UINT32 index) const noexcept { return Get()->GetValueSize(index); }

		[[nodiscard]] auto GetPropertyName(UINT32 index) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetSubProperty(UINT32 index) const noexcept -> Result<D2DProperties>;

		[[nodiscard]] auto GetProperty(UINT32 index, std::span<BYTE> bytes) const noexcept -> bool;

		template <typename T>
		[[nodiscard]] auto GetProperty(const UINT32 index) const noexcept -> Result<T>
		{
			T value{ };
			const auto size = GetValueSize(index);

			if (size != sizeof(T))
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Index", std::to_wstring(index))
					.AddDetail(L"sizeof T", std::to_wstring(sizeof(T)))
					.AddDetail(L"Size of value at index", std::to_wstring(size))
				};
			}

			if (const auto hr = this->Get()->GetValue(index, &value, size);
				FAILED(hr))
			{
				return Unexpected{ Error{ hr }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}
			return value;
		}

		template <typename T>
		[[nodiscard]] auto GetProperty(const std::wstring_view name) const noexcept -> Result<T>
		{
			const auto index = GetPropertyIndex(name);

			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Property Name", name)
				};
			}

			return GetProperty<T>(index);
		}

		template <>
		[[nodiscard]] auto GetProperty<bool>(const UINT32 index) const noexcept -> Result<bool>
		{
			bool value{ };
			if (const auto type = GetPropertyType(index);
				type != PropertyType::Bool)
			{
				return Unexpected{
						Error{ E_INVALIDARG }
						.AddDetail(L"Index", std::to_wstring(index))
				};
			}

			if (const auto hr = this->Get()->GetValue(index, &value);
				FAILED(hr))
			{
				return Unexpected{ Error{ hr }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}
			return value;
		}

		template <>
		[[nodiscard]] auto GetProperty<bool>(const std::wstring_view name) const noexcept -> Result<bool>
		{
			const auto index = GetPropertyIndex(name);

			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				return Unexpected{
						Error{ E_INVALIDARG }
						.AddDetail(L"Name", name)
				};
			}

			return GetProperty<bool>(index);
		}

		template <>
		[[nodiscard]] auto GetProperty<UINT32>(const UINT32 index) const noexcept -> Result<UINT32>
		{
			UINT32 value{ };
			if (const auto type = GetPropertyType(index);
				type != PropertyType::Bool)
			{
				return Unexpected{
						Error{ E_INVALIDARG }
						.AddDetail(L"Index", std::to_wstring(index))
				};
			}

			if (const auto hr = this->Get()->GetValue(index, &value);
				FAILED(hr))
			{
				return Unexpected{ Error{ hr }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}
			return value;
		}

		template <>
		[[nodiscard]] auto GetProperty<UINT32>(const std::wstring_view name) const noexcept -> Result<UINT32>
		{
			const auto index = GetPropertyIndex(name);
			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Name", name)
				};
			}
			return GetProperty<UINT32>(index);
		}

		template <>
		[[nodiscard]] auto GetProperty<INT32>(const UINT32 index) const noexcept -> Result<INT32>
		{
			INT32 value{ };
			if (const auto type = GetPropertyType(index); 
				type != PropertyType::Int32)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}

			if (const auto hr = Get()->GetValue(index, &value);
				FAILED(hr))
			{
				return Unexpected{ Error{ hr }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}
			return value;
		}

		template <>
		[[nodiscard]] auto GetProperty<INT32>(const std::wstring_view name) const noexcept -> Result<INT32>
		{
			const auto index = GetPropertyIndex(name);
			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Name", name)
				};
			}
			return GetProperty<INT32>(index);
		}

		template <>
		[[nodiscard]] auto GetProperty<float>(const UINT32 index) const noexcept -> Result<float>
		{
			FLOAT value{ };
			if (const auto type = GetPropertyType(index); 
				type != PropertyType::Float)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}

			if (const auto hr = Get()->GetValue(index, &value);
				FAILED(hr))
			{
				return Unexpected{ Error{ hr }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}
			return value;
		}

		template <>
		[[nodiscard]] auto GetProperty<float>(const std::wstring_view name) const noexcept -> Result<float>
		{
			const auto index = GetPropertyIndex(name);
			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				return Unexpected{
					Error{ E_INVALIDARG }
					.AddDetail(L"Name", name)
				};
			}
			return GetProperty<float>(index);
		}

		template <typename T>
		auto SetProperty(UINT32 index, const T& value) -> void
		{
			if (const auto hr = this->Get()->SetValue(index, value);
				FAILED(hr))
			{
				Logger::Log(
					L"Set property at index {} with value {}",
					index, value);
			}
		}

		template <typename T>
		auto SetProperty(const std::wstring_view name, const T& value) -> void
		{
			const auto index = GetPropertyIndex(name);

			if (index == D2D1_INVALID_PROPERTY_INDEX)
			{
				Logger::Log(
					L"Set property with name {} with value {}",
					name, value);
			}
			SetValue(index, value);
		}
	};
}
