module;
#include <d2d1_1.h>
#include <wrl.h>

module PGUI.UI.D2D.D2DProperties;

import std;

import PGUI.ComPtr;
import PGUI.Logging;

namespace PGUI::UI::D2D
{
	D2DProperties::D2DProperties(const ComPtr<ID2D1Properties>& properties) noexcept :
		ComPtrHolder{ properties }
	{ }

	auto D2DProperties::GetPropertyName(const UINT32 index) const noexcept -> std::wstring
	{
		auto& ptr = Get();
		const auto nameLength = ptr->GetPropertyNameLength(index) + 1;

		std::wstring name(nameLength, L'\0');
		const auto hr = ptr->GetPropertyName(index, name.data(), nameLength);
		LogFailed(LogLevel::Error, hr);

		return name;
	}

	auto D2DProperties::GetSubProperty(const UINT32 index) const noexcept -> D2DProperties
	{
		ComPtr<ID2D1Properties> subProperty;
		const auto hr = Get()->GetSubProperties(index, &subProperty);
		LogFailed(LogLevel::Error, hr);

		return D2DProperties{ subProperty };
	}

	auto D2DProperties::GetProperty(const UINT32 index, std::span<BYTE> bytes) const noexcept -> bool
	{
		if constexpr (bytes.extent == std::dynamic_extent)
		{
			const auto size = GetValueSize(index);
			if (size > bytes.size())
			{
				LogFailed(LogLevel::Error, E_INVALIDARG);
				return false;
			}
			const auto hr = Get()->GetValue(index, bytes.data(), size);
			LogFailed(LogLevel::Error, hr);
			return hr == S_OK;
		}
		else
		{
			auto hr = Get()->GetValue(index, bytes.data(), static_cast<UINT32>(bytes.size()));
			LogFailed(LogLevel::Error, hr);
			return hr == S_OK;
		}
	}
}
